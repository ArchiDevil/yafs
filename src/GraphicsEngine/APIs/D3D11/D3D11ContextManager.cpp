#include "D3D11ContextManager.h"

#include <RenderQueue.h>
#include <SceneGraph/SpriteSceneNode.h>
#include <SceneGraph/CameraSceneNode.h>
#include <IMeshData.h>

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include <sstream>

#include <D3Dcompiler.h>

using namespace MathLib;

namespace ShiftEngine
{

D3D11ContextManager::D3D11ContextManager(HWND hwnd, GraphicEngineSettings settings, PathSettings paths)
    : windowHandle(hwnd)
    , engineSettings(settings)
    , enginePaths(paths)
{
    if (enginePaths.FontsPath.empty() ||
        enginePaths.ShaderPath.empty() ||
        enginePaths.TexturePath.empty())
        LOG_ERROR("Some settings paths are not filled");

    graphicsContext = std::make_unique<D3D11Context>(windowHandle, engineSettings);
    textureManager = std::make_unique<D3D11TextureManager>(graphicsContext->Device, graphicsContext->DeviceContext, enginePaths.TexturePath);
    meshManager = std::make_unique<D3D11MeshManager>(graphicsContext->Device);
    fontManager = std::make_unique<FontManager>(this);
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
    fontManager->DrawAll(queue);

    BeginScene();

    decltype(auto) currentCamera = queue.GetActiveCamera();

    auto spriteSortFunctor = [](SpriteSceneNode* left, SpriteSceneNode* right) { return left->GetDrawingMode() < right->GetDrawingMode(); };

    if (!spriteVS || !spritePS)
        LoadSpritesPrerequisites();

    SetZState(true);
    SetRasterizerState(RasterizerState::NoCulling);

    SetUserDebugEventBegin(L"Sprites drawing");

    graphicsContext->DeviceContext->IASetInputLayout(spriteLayout.Get());

    graphicsContext->DeviceContext->VSSetShader(spriteVS.Get(), nullptr, 0);
    graphicsContext->DeviceContext->PSSetShader(spritePS.Get(), nullptr, 0);

    graphicsContext->DeviceContext->VSSetConstantBuffers(0, 1, spriteCB.GetAddressOf());
    graphicsContext->DeviceContext->PSSetConstantBuffers(0, 1, spriteCB.GetAddressOf());

    for (auto& spriteLayerPair : queue.GetSpriteNodes())
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

            if (currentBlendingState == BlendingState::AlphaEnabled && sprite->GetDrawingMode() == SpriteSceneNode::SpriteDrawingMode::Additive)
                SetBlendingState(BlendingState::Additive);

            const ITexturePtr& texture = sprite->GetTexture();

            if (!texture)
                continue;

            const vec4f & maskColor = sprite->GetMaskColor();
            const mat4f & matWorld = sprite->GetWorldMatrix();
            mat4f matResult = matWorld * currentCamera.GetViewMatrix() * currentCamera.GetProjectionMatrix();

            //TODO: ugly-ugly shit, need to do something with it D:<
            SpriteCB cbufferToFill;
            memcpy(cbufferToFill.WVPMatrix, (float*)matResult, sizeof(float) * 16);
            memcpy(cbufferToFill.TextureMatrix[0], sprite->GetTextureMatrix()[0], sizeof(float) * 3);
            memcpy(cbufferToFill.TextureMatrix[1], sprite->GetTextureMatrix()[1], sizeof(float) * 3);
            memcpy(cbufferToFill.TextureMatrix[2], sprite->GetTextureMatrix()[2], sizeof(float) * 3);
            memcpy(cbufferToFill.MaskColor, maskColor.el, sizeof(float) * 4);

            D3D11_MAPPED_SUBRESOURCE ss;
            graphicsContext->DeviceContext->Map(spriteCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ss);
            memcpy(ss.pData, &cbufferToFill, sizeof(SpriteCB));
            graphicsContext->DeviceContext->Unmap(spriteCB.Get(), 0);

            texture->Bind(0, BindingPoint::Pixel);
            DrawMesh(spriteMesh);
        }
    }

    SetUserDebugEventEnd();

    SetUserDebugEventBegin(L"Text drawing");

    for (auto& textNode : queue.GetTextNodes())
    {
        ITexturePtr texture = textNode->GetTexture();

        if (!texture)
            continue;

        constexpr vec4f maskColor = { 1.0, 1.0, 1.0, 1.0 };
        const mat3f textureMatrix = matrixIdentity<float, 3>();

        mat4f position = matrixTranslation(vec3f{ textNode->GetPosition().x, textNode->GetPosition().y, 0.0 });
        mat4f projectionMatrix = matrixOrthoOffCenterLH<float>(0.0f, (float)engineSettings.screenWidth, (float)engineSettings.screenHeight, 0.0f, 0.0f, 1.0f);
        mat4f matResult = position * projectionMatrix;

        //TODO: ugly-ugly shit, need to do something with it D:<
        SpriteCB cbufferToFill;
        memcpy(cbufferToFill.WVPMatrix, (float*)matResult, sizeof(float) * 16);
        memcpy(cbufferToFill.TextureMatrix[0], textureMatrix[0], sizeof(float) * 3);
        memcpy(cbufferToFill.TextureMatrix[1], textureMatrix[1], sizeof(float) * 3);
        memcpy(cbufferToFill.TextureMatrix[2], textureMatrix[2], sizeof(float) * 3);
        memcpy(cbufferToFill.MaskColor, maskColor.el, sizeof(float) * 4);

        D3D11_MAPPED_SUBRESOURCE ss;
        graphicsContext->DeviceContext->Map(spriteCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ss);
        memcpy(ss.pData, &cbufferToFill, sizeof(SpriteCB));
        graphicsContext->DeviceContext->Unmap(spriteCB.Get(), 0);

        texture->Bind(0, BindingPoint::Pixel);
        IMeshDataPtr mesh = textNode->GetMesh();
        if (mesh)
            DrawMesh(mesh);
    }

    SetUserDebugEventEnd();

    EndScene();
}

void D3D11ContextManager::BeginScene()
{
    SetUserDebugEventBegin(L"RTs clear");
    graphicsContext->ClearDefaultRenderTarget();
    SetUserDebugEventEnd();
}

void D3D11ContextManager::EndScene()
{
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

int D3D11ContextManager::DrawMesh(const IMeshDataPtr& mesh)
{
    return mesh->Draw();
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

FontManager * D3D11ContextManager::GetFontManager()
{
    return fontManager.get();
}

ITextureManager * D3D11ContextManager::GetTextureManager()
{
    return textureManager.get();
}

ShiftEngine::IMeshManager * D3D11ContextManager::GetMeshManager()
{
    return meshManager.get();
}

void D3D11ContextManager::LoadSpritesPrerequisites()
{
    uint32_t compileFlags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
    compileFlags |= D3DCOMPILE_ENABLE_STRICTNESS;
    compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
    compileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    std::string filename = utils::narrow(enginePaths.ShaderPath + L"sprite.hlsl");
    std::ifstream in(filename);
    if (in.fail())
        LOG_FATAL_ERROR("Unable to open ", filename);

    std::stringstream buffer;
    buffer << in.rdbuf();

    Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errors = nullptr;
    HRESULT hr = D3DCompile(buffer.str().c_str(), buffer.str().size(), nullptr, nullptr, nullptr, "PS", "ps_4_0", compileFlags, 0, &blob, &errors);
    if (FAILED(hr))
    {
        LOG_ERROR("Internal error while compiling sprite shader: ", std::hex, hr, std::dec);
        const char* errorMsg = (const char*)errors->GetBufferPointer();
        LOG_FATAL_ERROR(errorMsg);
    }

    hr = graphicsContext->Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &spritePS);
    if (FAILED(hr))
    {
        LOG_FATAL_ERROR("Internal error while compiling sprite shader: ", std::hex, hr, std::dec);
    }

    hr = D3DCompile(buffer.str().c_str(), buffer.str().size(), nullptr, nullptr, nullptr, "VS", "vs_4_0", compileFlags, 0, &blob, &errors);
    if (FAILED(hr))
    {
        LOG_ERROR("Internal error while compiling sprite shader: ", std::hex, hr, std::dec);
        const char* errorMsg = (const char*)errors->GetBufferPointer();
        LOG_FATAL_ERROR(errorMsg);
    }

    hr = graphicsContext->Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &spriteVS);
    if (FAILED(hr))
    {
        LOG_FATAL_ERROR("Internal error while compiling sprite shader: ", std::hex, hr, std::dec);
    }

    D3D11_BUFFER_DESC desc;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = sizeof(SpriteCB);
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.Usage = D3D11_USAGE_DYNAMIC;

    hr = graphicsContext->Device->CreateBuffer(&desc, nullptr, &spriteCB);
    if (FAILED(hr))
    {
        LOG_FATAL_ERROR("Internal error: ", std::hex, hr, std::dec);
    }

    D3D11_INPUT_ELEMENT_DESC inputElements[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = graphicsContext->Device->CreateInputLayout(inputElements, sizeof(inputElements) / sizeof(inputElements[0]), blob->GetBufferPointer(), blob->GetBufferSize(), &spriteLayout);
    if (FAILED(hr))
    {
        LOG_FATAL_ERROR("Internal error: ", std::hex, hr, std::dec);
    }

    std::vector<SpriteVertex> vertices =
    {
        { { -0.5f, -0.5f }, { 0.0f, 0.0f } },
        { { 0.5f, -0.5f },  { 1.0f, 0.0f } },
        { { -0.5f, 0.5f },  { 0.0f, 1.0f } },

        { { 0.5f, -0.5f },  { 1.0f, 0.0f } },
        { { 0.5f, 0.5f },   { 1.0f, 1.0f } },
        { { -0.5f, 0.5f },  { 0.0f, 1.0f } }
    };

    spriteMesh = meshManager->CreateMeshFromVertices(vertices, {}, {});
}

void D3D11ContextManager::SetWireframeState(bool state)
{
    SetRasterizerState(state ? RasterizerState::Wireframe : RasterizerState::Normal);
}

}
