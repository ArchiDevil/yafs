#include "D3D11Context.h"

#include <Utilities/logger.hpp>

using namespace ShiftEngine;

ShiftEngine::D3D11Context::D3D11Context(HWND windowHandle, const GraphicEngineSettings &settings)
{
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    desc.BufferDesc.Height = settings.screenHeight;
    desc.BufferDesc.Width = settings.screenWidth;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.RefreshRate.Numerator = settings.screenRate;

    desc.SampleDesc.Count = settings.multisampleQuality;
    desc.SampleDesc.Quality = 0;

    desc.BufferCount = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.Flags = 0;
    desc.OutputWindow = windowHandle;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Windowed = settings.windowed;

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
                                               &SwapChain,
                                               &Device,
                                               &featureLevel,
                                               &DeviceContext
    );

    if (FAILED(hr))
        LOG_FATAL_ERROR("Unable to create D3D11 device: ", std::hex, hr, std::dec);

    if (featureLevel != D3D_FEATURE_LEVEL_11_0)
        LOG_ERROR("Unable to create device with DX11 feature level, further work may be unstable");

    LOG_INFO("Device created");

    ID3D11Texture2D * tempTex = nullptr;

    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tempTex));
    Device->CreateRenderTargetView(tempTex, 0, &DefaultRT->view);
    tempTex->Release();

    D3D11_TEXTURE2D_DESC depthStencilDesc;                              //описание глубинно-стенсильного буфера
    depthStencilDesc.Width = settings.screenWidth;                //ширина
    depthStencilDesc.Height = settings.screenHeight;              //высота
    depthStencilDesc.MipLevels = 1;                                     //мип-уровни
    depthStencilDesc.ArraySize = 1;                                     //размер текстуры
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;            //формат
    depthStencilDesc.SampleDesc.Count = settings.multisampleQuality;    //multisampling must match
    depthStencilDesc.SampleDesc.Quality = 0;                            //swap chain values
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;                       //использование
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;              //флаги, куда прибиндивать
    depthStencilDesc.CPUAccessFlags = 0;                                //доступ процессора
    depthStencilDesc.MiscFlags = 0;                                     //прочие флаги

    if (FAILED(Device->CreateTexture2D(&depthStencilDesc, 0, &DefaultDS->texture)))
        LOG_FATAL_ERROR("Unable to create default depth buffer");

    if (FAILED(Device->CreateDepthStencilView(DefaultDS->texture.Get(), 0, &DefaultDS->view)))
        LOG_FATAL_ERROR("Unable to create default depth buffer");

    DeviceContext->OMSetRenderTargets(1, DefaultRT->view.GetAddressOf(), DefaultDS->view.Get());

    D3D11_VIEWPORT vp{
        vp.TopLeftX = 0,
        vp.TopLeftY = 0,
        vp.Width = (float)settings.screenWidth,
        vp.Height = (float)settings.screenHeight,
        vp.MinDepth = 0.0f,
        vp.MaxDepth = 1.0f
    };

    DeviceContext->RSSetViewports(1, &vp);

#if defined DEBUG || defined _DEBUG
    if (FAILED(CreateAnnotationsHandler()))
        LOG_FATAL_ERROR("Unable to annotations handler for renderer");
#endif

    if (FAILED(CreateStates()))
        LOG_FATAL_ERROR("Unable to create states for renderer");

    DeviceContext->OMSetDepthStencilState(dsStateZOn.Get(), 1);
    DeviceContext->RSSetState(rsNormal.Get());
    const float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    DeviceContext->OMSetBlendState(bsNormal.Get(), BlendFactor, 0xffffffff);

    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //HACK: LOADING SAMPLERS
    D3D11_SAMPLER_DESC sDesc;
    sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.BorderColor[0] = sDesc.BorderColor[1] = sDesc.BorderColor[2] = sDesc.BorderColor[3] = 0.0f;
    sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    if (settings.anisotropyLevel > 0)
    {
        sDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        sDesc.MaxAnisotropy = settings.anisotropyLevel;
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
    hr = Device->CreateSamplerState(&sDesc, &sampler);
    if (FAILED(hr))
        LOG_FATAL_ERROR("Unable to create samplers, error code: ", std::to_string(hr));

    DeviceContext->PSSetSamplers(0, 1, &sampler);
}

D3D11Context::~D3D11Context()
{
    if (DeviceContext)
        DeviceContext->ClearState();

    // make sure here that objects are released in appropriate order
    // to avoid crashes and asserts in DX runtime
    bsAlpha.Reset();
    bsAdditive.Reset();
    bsNormal.Reset();

    rsNormal.Reset();
    rsWireframe.Reset();
    rsNoCulling.Reset();

    dsStateZOn.Reset();
    dsStateZOff.Reset();

    DefaultDS.reset(nullptr);
    DefaultRT.reset(nullptr);
    DeviceContext.Reset();
    SwapChain.Reset();
    Device.Reset();
}

void D3D11Context::ClearDefaultRenderTarget()
{
    static float clearColors[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    DeviceContext->ClearRenderTargetView(DefaultRT->view.Get(), clearColors);
    DeviceContext->ClearDepthStencilView(DefaultDS->view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

HRESULT D3D11Context::CreateStates()
{
    D3D11_BLEND_DESC bdDescNormal;
    ZeroMemory(&bdDescNormal, sizeof(D3D11_BLEND_DESC));
    for (auto & i : bdDescNormal.RenderTarget)
    {
        i.BlendOp = D3D11_BLEND_OP_MAX;
        i.DestBlend = D3D11_BLEND_DEST_COLOR;
        i.SrcBlend = D3D11_BLEND_SRC_COLOR;
        i.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    HRESULT hr = Device->CreateBlendState(&bdDescNormal, &bsNormal);
    if (FAILED(hr))
        return hr;

    D3D11_BLEND_DESC bdDescAlpha;
    ZeroMemory(&bdDescAlpha, sizeof(D3D11_BLEND_DESC));
    bdDescAlpha.AlphaToCoverageEnable = false;
    for (auto & i : bdDescAlpha.RenderTarget)
    {
        i.BlendEnable = true;

        i.BlendOp = D3D11_BLEND_OP_ADD;
        i.BlendOpAlpha = D3D11_BLEND_OP_ADD;

        i.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        i.DestBlendAlpha = D3D11_BLEND_ONE;

        i.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        i.SrcBlendAlpha = D3D11_BLEND_ONE;

        i.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    hr = Device->CreateBlendState(&bdDescAlpha, &bsAlpha);
    if (FAILED(hr))
        return hr;

    D3D11_BLEND_DESC bdDescAdditive;
    ZeroMemory(&bdDescAdditive, sizeof(D3D11_BLEND_DESC));
    bdDescAdditive.AlphaToCoverageEnable = false;
    for (auto & i : bdDescAdditive.RenderTarget)
    {
        i.BlendEnable = true;

        i.BlendOp = D3D11_BLEND_OP_ADD;
        i.BlendOpAlpha = D3D11_BLEND_OP_ADD;

        i.DestBlend = D3D11_BLEND_ONE;
        i.DestBlendAlpha = D3D11_BLEND_ONE;

        i.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        i.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;

        i.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    hr = Device->CreateBlendState(&bdDescAdditive, &bsAdditive);
    if (FAILED(hr))
        return hr;

    D3D11_DEPTH_STENCIL_DESC dsDescZOn;
    ZeroMemory(&dsDescZOn, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsDescZOn.DepthEnable = true;
    dsDescZOn.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDescZOn.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    hr = Device->CreateDepthStencilState(&dsDescZOn, &dsStateZOn);
    if (FAILED(hr))
        return hr;

    D3D11_DEPTH_STENCIL_DESC dsDescZOff;
    ZeroMemory(&dsDescZOff, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsDescZOff.DepthEnable = false;
    hr = Device->CreateDepthStencilState(&dsDescZOff, &dsStateZOff);
    if (FAILED(hr))
        return hr;

    D3D11_RASTERIZER_DESC rsDescNormal;
    ZeroMemory(&rsDescNormal, sizeof(D3D11_RASTERIZER_DESC));
    rsDescNormal.FillMode = D3D11_FILL_SOLID;
    rsDescNormal.CullMode = D3D11_CULL_BACK;
    rsDescNormal.FrontCounterClockwise = FALSE;
    rsDescNormal.MultisampleEnable = TRUE;
    rsDescNormal.DepthClipEnable = TRUE;
    hr = Device->CreateRasterizerState(&rsDescNormal, &rsNormal);
    if (FAILED(hr))
        return false;

    D3D11_RASTERIZER_DESC rsDescWireframe;
    ZeroMemory(&rsDescWireframe, sizeof(D3D11_RASTERIZER_DESC));
    rsDescWireframe.FillMode = D3D11_FILL_WIREFRAME;
    rsDescWireframe.CullMode = D3D11_CULL_NONE;
    rsDescWireframe.FrontCounterClockwise = FALSE;
    rsDescWireframe.MultisampleEnable = TRUE;
    rsDescWireframe.DepthClipEnable = TRUE;
    hr = Device->CreateRasterizerState(&rsDescWireframe, &rsWireframe);
    if (FAILED(hr))
        return hr;

    D3D11_RASTERIZER_DESC rsDescNoCulling;
    ZeroMemory(&rsDescNoCulling, sizeof(D3D11_RASTERIZER_DESC));
    rsDescNoCulling.FillMode = D3D11_FILL_SOLID;
    rsDescNoCulling.CullMode = D3D11_CULL_NONE;
    rsDescNoCulling.FrontCounterClockwise = FALSE;
    rsDescNoCulling.MultisampleEnable = TRUE;
    rsDescNoCulling.DepthClipEnable = TRUE;
    hr = Device->CreateRasterizerState(&rsDescNoCulling, &rsNoCulling);
    if (FAILED(hr))
        return hr;

    return true;
}

HRESULT ShiftEngine::D3D11Context::CreateAnnotationsHandler()
{
    return DeviceContext.CopyTo(__uuidof(ID3DUserDefinedAnnotation), (void**)&UserAnnotationsHandler);
}
