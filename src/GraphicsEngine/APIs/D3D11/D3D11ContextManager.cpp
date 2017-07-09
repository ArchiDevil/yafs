#include "D3D11ContextManager.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include <sstream>

#include <D3Dcompiler.h>

using namespace ShiftEngine;

D3D11ContextManager::D3D11ContextManager(HWND hwnd)
    : windowHandle(hwnd)
{
}

ShiftEngine::D3D11ContextManager::~D3D11ContextManager()
{}

void ShiftEngine::D3D11ContextManager::SetUserDebugMarker(const std::wstring & markerName)
{
#if defined DEBUG || _DEBUG
    graphicsContext.UserAnnotationsHandler->SetMarker(markerName.c_str());
#else
    markerName;
#endif
}

void ShiftEngine::D3D11ContextManager::SetUserDebugEventBegin(const std::wstring & markerName)
{
#if defined DEBUG || _DEBUG
    graphicsContext.UserAnnotationsHandler->BeginEvent(markerName.c_str());
#else
    markerName;
#endif
}

void ShiftEngine::D3D11ContextManager::SetUserDebugEventEnd()
{
#if defined DEBUG || _DEBUG
    graphicsContext.UserAnnotationsHandler->EndEvent();
#endif
}

void ShiftEngine::D3D11ContextManager::DrawAll(RenderQueue& queue, double dt)
{

}

bool D3D11ContextManager::Initialize(GraphicEngineSettings _Settings, PathSettings _Paths)
{
    engineSettings = _Settings;
    enginePaths = _Paths;

    if (enginePaths.FontsPath.empty() ||
        enginePaths.ShaderPath.empty() ||
        enginePaths.TexturePath.empty())
        LOG_ERROR("Some settings paths are not filled");

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;                   //формат буферов
    desc.BufferDesc.Height = engineSettings.screenHeight;                       //высота
    desc.BufferDesc.Width = engineSettings.screenWidth;                         //ширина
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                    //масштабирование?
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;    //развертка строк?
    desc.BufferDesc.RefreshRate.Denominator = 1;                                //знаменатель?
    desc.BufferDesc.RefreshRate.Numerator = engineSettings.screenRate;          //частота обновления экрана

    desc.SampleDesc.Count = engineSettings.multisampleQuality;                  //мультисемплинг 
    desc.SampleDesc.Quality = 0;                                                //качество его

    desc.BufferCount = 1;                                                       //кол-во задних буферов - один, за глаза
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                         //использование буфера
    desc.Flags = 0;                                                             //флаги
    desc.OutputWindow = windowHandle;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Windowed = engineSettings.windowed;

    unsigned int Flags = 0;
#if defined (DEBUG) || (_DEBUG)
    Flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_1,
    };

    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
                                               D3D_DRIVER_TYPE_HARDWARE,
                                               NULL,
                                               Flags,
                                               featureLevels,
                                               3,
                                               D3D11_SDK_VERSION,
                                               &desc,
                                               &graphicsContext.SwapChain,
                                               &graphicsContext.Device,
                                               &featureLevel,
                                               &graphicsContext.DeviceContext
    );

    if (FAILED(hr))
        LOG_FATAL_ERROR("Unable to create D3D11 device: ", std::hex, hr, std::dec);

    if (featureLevel != D3D_FEATURE_LEVEL_11_0)
        LOG_ERROR("Unable to create device with DX11 feature level, further work may be unstable");

    LOG_INFO("Device created");

    ID3D11Texture2D * tempTex = nullptr;

    graphicsContext.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tempTex));
    graphicsContext.Device->CreateRenderTargetView(tempTex, 0, &graphicsContext.DefaultRT->view);
    tempTex->Release();

    /////////////////////////////////////
    // Создание депт и стенсил буферов //
    /////////////////////////////////////

    D3D11_TEXTURE2D_DESC depthStencilDesc;                              //описание глубинно-стенсильного буфера
    depthStencilDesc.Width = engineSettings.screenWidth;                //ширина
    depthStencilDesc.Height = engineSettings.screenHeight;              //высота
    depthStencilDesc.MipLevels = 1;                                     //мип-уровни
    depthStencilDesc.ArraySize = 1;                                     //размер текстуры
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;            //формат
    depthStencilDesc.SampleDesc.Count = engineSettings.multisampleQuality;    //multisampling must match
    depthStencilDesc.SampleDesc.Quality = 0;                            //swap chain values
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;                       //использование
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;              //флаги, куда прибиндивать
    depthStencilDesc.CPUAccessFlags = 0;                                //доступ процессора
    depthStencilDesc.MiscFlags = 0;                                     //прочие флаги

    if (FAILED(graphicsContext.Device->CreateTexture2D(&depthStencilDesc, 0, &graphicsContext.DefaultDS->texture)))
        LOG_FATAL_ERROR("Unable to create default depth buffer");

    if (FAILED(graphicsContext.Device->CreateDepthStencilView(graphicsContext.DefaultDS->texture.Get(), 0, &graphicsContext.DefaultDS->view)))
        LOG_FATAL_ERROR("Unable to create default depth buffer");

    graphicsContext.DeviceContext->OMSetRenderTargets(1, graphicsContext.DefaultRT->view.GetAddressOf(), graphicsContext.DefaultDS->view.Get());

    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = (float)engineSettings.screenWidth;
    vp.Height = (float)engineSettings.screenHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    graphicsContext.DeviceContext->RSSetViewports(1, &vp);

#if defined DEBUG || defined _DEBUG
    if(FAILED(graphicsContext.CreateAnnotationsHandler()))
        LOG_FATAL_ERROR("Unable to annotations handler for renderer");
#endif

    if (FAILED(graphicsContext.CreateStates()))
        LOG_FATAL_ERROR("Unable to create states for renderer");

    zBufferState = true;
    graphicsContext.DeviceContext->OMSetDepthStencilState(graphicsContext.dsStateZOn.Get(), 1);
    graphicsContext.DeviceContext->RSSetState(graphicsContext.rsNormal.Get());
    const float BlendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsNormal.Get(), BlendFactor, 0xffffffff);

    textureManager = new D3D11TextureManager(graphicsContext.Device, graphicsContext.DeviceContext, enginePaths.TexturePath);
    // fontManager = new FontManager();

    graphicsContext.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //HACK: LOADING SAMPLERS
    D3D11_SAMPLER_DESC sDesc;
    sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.BorderColor[0] = sDesc.BorderColor[1] = sDesc.BorderColor[2] = sDesc.BorderColor[3] = 0.0f;
    sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    if (engineSettings.anisotropyLevel > 0)
    {
        sDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        sDesc.MaxAnisotropy = engineSettings.anisotropyLevel;
    }
    else
    {
        sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sDesc.MaxAnisotropy = 0;
    }
    sDesc.MaxLOD = 8;
    sDesc.MinLOD = 0.0f;
    sDesc.MipLODBias = 0.0f;
    ID3D11SamplerState * sampler = nullptr;
    hr = graphicsContext.Device->CreateSamplerState(&sDesc, &sampler);
    if (FAILED(hr))
        LOG_FATAL_ERROR("Unable to create samplers, error code: ", std::to_string(hr));

    graphicsContext.DeviceContext->PSSetSamplers(0, 1, &sampler);

    return true;
}

std::wstring D3D11ContextManager::GetGPUDescription()
{
    DXGI_ADAPTER_DESC adapterDesc;
    IDXGIFactory * factory;
    IDXGIAdapter * adapter;
    // Create a DirectX graphics interface factory.
    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    // Use the factory to create an adapter for the primary graphics interface (video card).
    factory->EnumAdapters(0, &adapter);
    adapter->GetDesc(&adapterDesc);
    std::wstring buffer = adapterDesc.Description;
    factory->Release();
    adapter->Release();
    return buffer;
}

void D3D11ContextManager::BeginScene()
{
    SetUserDebugEventBegin(L"RTs clear");
    graphicsContext.ClearDefaultRenderTarget();
    SetUserDebugEventEnd();
}

void D3D11ContextManager::EndScene()
{
    SetUserDebugEventBegin(L"Text drawing");
    //fontManager->DrawBatchedText();
    SetUserDebugEventEnd();

    if (engineSettings.screenRate > 0)
        graphicsContext.SwapChain->Present(1, 0);
    else
        graphicsContext.SwapChain->Present(0, 0);
}

void D3D11ContextManager::ResetPipeline()
{
    UINT null = 0;
    ID3D11Buffer* nullB = nullptr;
    graphicsContext.DeviceContext->IASetVertexBuffers(0, 1, &nullB, &null, &null);
    graphicsContext.DeviceContext->IASetIndexBuffer(nullB, DXGI_FORMAT_UNKNOWN, NULL);
    graphicsContext.DeviceContext->IASetInputLayout(nullptr);
}

ITexturePtr D3D11ContextManager::LoadTexture(const std::wstring & FileName)
{
    ITexturePtr out = textureManager->CreateTexture2D(FileName);
    if (out == nullptr)
    {
        LOG_ERROR("Unable to load ", utils::narrow(FileName), ", loaded empty texture.");
        out = textureManager->GetErrorTexture();
    }
    return out;
}

void D3D11ContextManager::SetZState(bool enabled)
{
    zBufferState = enabled;
    if (enabled)
        graphicsContext.DeviceContext->OMSetDepthStencilState(graphicsContext.dsStateZOn.Get(), 1);
    else
        graphicsContext.DeviceContext->OMSetDepthStencilState(graphicsContext.dsStateZOff.Get(), 1);
}

const GraphicEngineSettings & D3D11ContextManager::GetEngineSettings() const
{
    return engineSettings;
}

const PathSettings & D3D11ContextManager::GetPaths() const
{
    return enginePaths;
}

int D3D11ContextManager::DrawMesh()
{
    //if (mesh && mesh->GetVertexDeclaration())
    //{
    //    if (mesh->GetVertexDeclaration().get() != currentVertexDeclaration)
    //    {
    //        mesh->GetVertexDeclaration()->Bind();
    //        currentVertexDeclaration = mesh->GetVertexDeclaration().get();
    //    }
    //    return mesh->Draw();
    //}
    //else
    //{
    //    return 0;
    //}
    return 0;
}

void D3D11ContextManager::SetBlendingState(BlendingState bs)
{
    const float BlendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    switch (bs)
    {
    case BlendingState::None:
        graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsNormal.Get(), BlendFactor, 0xffffffff);
        break;
    case BlendingState::AlphaEnabled:
        graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsAlpha.Get(), BlendFactor, 0xffffffff);
        break;
    case BlendingState::Additive:
        graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsAdditive.Get(), BlendFactor, 0xffffffff);
        break;
    default:
        break;
    }

    currentBlendingState = bs;
}

void D3D11ContextManager::SetRasterizerState(RasterizerState rs)
{
    currentRasterizerState = rs;
    switch (rs)
    {
    case RasterizerState::Wireframe:
        graphicsContext.DeviceContext->RSSetState(graphicsContext.rsWireframe.Get());
        break;
    case RasterizerState::Normal:
        graphicsContext.DeviceContext->RSSetState(graphicsContext.rsNormal.Get());
        break;
    case RasterizerState::NoCulling:
        graphicsContext.DeviceContext->RSSetState(graphicsContext.rsNoCulling.Get());
        break;
    default:
        break;
    }
}

//FontManager * D3D11ContextManager::GetFontManager()
//{
//    return fontManager;
//}

ITextureManager * D3D11ContextManager::GetTextureManager()
{
    return textureManager;
}

ID3D11Device* D3D11ContextManager::GetDevicePtr() const
{
    return graphicsContext.Device.Get();
}
