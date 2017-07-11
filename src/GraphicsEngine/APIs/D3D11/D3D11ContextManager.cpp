#include "D3D11ContextManager.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include <sstream>

#include <D3Dcompiler.h>

namespace ShiftEngine
{

D3D11ContextManager::D3D11ContextManager(HWND hwnd)
    : windowHandle(hwnd)
{
}

void D3D11ContextManager::SetUserDebugMarker(const std::wstring & markerName)
{
#if defined DEBUG || _DEBUG
    graphicsContext->UserAnnotationsHandler->SetMarker(markerName.c_str());
#else
    markerName;
#endif
}

void D3D11ContextManager::SetUserDebugEventBegin(const std::wstring & markerName)
{
#if defined DEBUG || _DEBUG
    graphicsContext->UserAnnotationsHandler->BeginEvent(markerName.c_str());
#else
    markerName;
#endif
}

void D3D11ContextManager::SetUserDebugEventEnd()
{
#if defined DEBUG || _DEBUG
    graphicsContext->UserAnnotationsHandler->EndEvent();
#endif
}

void D3D11ContextManager::DrawAll(RenderQueue& queue, double dt)
{
    struct textureMatrixWithPadding
    {
        // this is 3x3 matrix and this requires 
        // padding (16 bytes alignment) for HLSL shader
        alignas(16) float firstRow[3];
        alignas(16) float secondRow[3];
        alignas(16) float thirdRow[3];
    };

    auto spriteSortFunctor = [](SpriteSceneNode* left, SpriteSceneNode* right) { return left->GetDrawingMode() < right->GetDrawingMode(); };

    if (!spriteProgram || !spriteMesh)
        loadSpritesPrerequisites();

    SetZState(true);
    SetRasterizerState(RasterizerState::NoCulling);

    SetUserDebugEventBegin(L"Sprites drawing");

    for (auto& spriteLayerPair : sprites)
    {
        std::sort(spriteLayerPair.second.begin(), spriteLayerPair.second.end(), spriteSortFunctor);
        SetBlendingState(BlendingState::AlphaEnabled);

        for (const SpriteSceneNode* sprite : spriteLayerPair.second)
        {
            if (!sprite)
            {
                LOG_ERROR("Empty sprite node in render queue");
                continue;
            }

            if (contextMgr->GetBlendingState() == BlendingState::AlphaEnabled && sprite->GetDrawingMode() == SpriteSceneNode::SpriteDrawingMode::Additive)
                SetBlendingState(BlendingState::Additive);

            const ITexturePtr & texture = sprite->GetTexture();

            if (!texture)
                return;

            const vec4f & maskColor = sprite->GetMaskColor();
            const mat4f & matWorld = sprite->GetWorldMatrix();
            mat4f matResult = currentCamera.GetViewMatrix() * currentCamera.GetProjectionMatrix();
            matResult = matWorld * matResult;

            spriteProgram->SetMatrixConstantByName("WVPMatrix", (float*)matResult);

            //TODO: ugly-ugly shit, need to do something with it D:<
            textureMatrixWithPadding texMatrix;
            memcpy(texMatrix.firstRow, sprite->GetTextureMatrix()[0], sizeof(float) * 3);
            memcpy(texMatrix.secondRow, sprite->GetTextureMatrix()[1], sizeof(float) * 3);
            memcpy(texMatrix.thirdRow, sprite->GetTextureMatrix()[2], sizeof(float) * 3);

            spriteProgram->SetMatrixConstantByName("TextureMatrix", (float*)&texMatrix);
            spriteProgram->SetTextureByIndex(0, texture); // there is only one texture for sprites
            spriteProgram->SetVectorConstantByName("MaskColor", maskColor.ptr());
            DrawMesh(spriteMesh);
        }
    }

    SetUserDebugEventEnd();
}

bool D3D11ContextManager::Initialize(GraphicEngineSettings _Settings, PathSettings _Paths)
{
    engineSettings = _Settings;
    enginePaths = _Paths;

    if (enginePaths.FontsPath.empty() ||
        enginePaths.ShaderPath.empty() ||
        enginePaths.TexturePath.empty())
        LOG_ERROR("Some settings paths are not filled");

    graphicsContext = std::make_unique<D3D11Context>(windowHandle);
    zBufferState = true;

    textureManager = new D3D11TextureManager(graphicsContext->Device, graphicsContext->DeviceContext, enginePaths.TexturePath);
    // fontManager = new FontManager();

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
    graphicsContext->ClearDefaultRenderTarget();
    SetUserDebugEventEnd();
}

void D3D11ContextManager::EndScene()
{
    SetUserDebugEventBegin(L"Text drawing");
    //fontManager->DrawBatchedText();
    SetUserDebugEventEnd();

    if (engineSettings.screenRate > 0)
        graphicsContext->SwapChain->Present(1, 0);
    else
        graphicsContext->SwapChain->Present(0, 0);
}

void D3D11ContextManager::ResetPipeline()
{
    UINT null = 0;
    ID3D11Buffer* nullB = nullptr;
    graphicsContext->DeviceContext->IASetVertexBuffers(0, 1, &nullB, &null, &null);
    graphicsContext->DeviceContext->IASetIndexBuffer(nullB, DXGI_FORMAT_UNKNOWN, NULL);
    graphicsContext->DeviceContext->IASetInputLayout(nullptr);
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
        graphicsContext->DeviceContext->OMSetDepthStencilState(graphicsContext->dsStateZOn.Get(), 1);
    else
        graphicsContext->DeviceContext->OMSetDepthStencilState(graphicsContext->dsStateZOff.Get(), 1);
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
    const float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    switch (bs)
    {
    case BlendingState::None:
        graphicsContext->DeviceContext->OMSetBlendState(graphicsContext->bsNormal.Get(), BlendFactor, 0xffffffff);
        break;
    case BlendingState::AlphaEnabled:
        graphicsContext->DeviceContext->OMSetBlendState(graphicsContext->bsAlpha.Get(), BlendFactor, 0xffffffff);
        break;
    case BlendingState::Additive:
        graphicsContext->DeviceContext->OMSetBlendState(graphicsContext->bsAdditive.Get(), BlendFactor, 0xffffffff);
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
        graphicsContext->DeviceContext->RSSetState(graphicsContext->rsWireframe.Get());
        break;
    case RasterizerState::Normal:
        graphicsContext->DeviceContext->RSSetState(graphicsContext->rsNormal.Get());
        break;
    case RasterizerState::NoCulling:
        graphicsContext->DeviceContext->RSSetState(graphicsContext->rsNoCulling.Get());
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

}
