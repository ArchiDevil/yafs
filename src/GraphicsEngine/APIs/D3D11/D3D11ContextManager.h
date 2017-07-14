#pragma once

#include "D3D11Context.h"
#include "D3D11TextureManager.h"
#include "D3D11RenderTarget.h"
#include "D3D11MeshManager.h"

#include <IContextManager.h>
#include <MiscTypes.h>
// #include "../../FontManager.h"

#include <exception>
#include <vector>
#include <memory>

namespace ShiftEngine
{

enum class BlendingState
{
    None,
    AlphaEnabled,
    Additive
};

enum class RasterizerState
{
    Wireframe,
    Normal,
    NoCulling
};

class D3D11ContextManager final : public IContextManager
{
public:
    D3D11ContextManager(HWND hwnd);

    bool                                Initialize(GraphicEngineSettings _Settings, PathSettings _Paths) override;
    ITexturePtr                         LoadTexture(const std::wstring & FileName) override;
    ITextureManager *                   GetTextureManager() override;
    IMeshManager *                      GetMeshManager() override;
    // FontManager*                        GetFontManager() override;
    const GraphicEngineSettings &       GetEngineSettings() const override;
    const PathSettings &                GetPaths() const override;
    void                                DrawAll(RenderQueue& queue, double dt) override;
    void                                SetWireframeState(bool state) override;

private:
    void                                BeginScene();
    void                                EndScene();
    void                                ResetPipeline();
    void                                SetUserDebugMarker(const std::wstring & markerName);
    void                                SetUserDebugEventBegin(const std::wstring & markerName);
    void                                SetUserDebugEventEnd();
    void                                SetZState(bool enabled);
    void                                SetBlendingState(BlendingState bs);
    void                                SetRasterizerState(RasterizerState rs);
    int                                 DrawSprite();
    void                                LoadSpritesPrerequisites();

    HWND                                        windowHandle;
    PathSettings                                enginePaths;
    GraphicEngineSettings                       engineSettings;

    std::unique_ptr<D3D11Context>               graphicsContext;
    // FontManager*                        fontManager = nullptr;
    std::unique_ptr<D3D11TextureManager>        textureManager = nullptr;
    std::unique_ptr<D3D11MeshManager>           meshManager = nullptr;

    RasterizerState                             currentRasterizerState = RasterizerState::Normal;
    BlendingState                               currentBlendingState = BlendingState::None;

    bool                                        zBufferState = true;
    bool                                        cullingEnabled = true;

    struct SpriteCB
    {
        float WVPMatrix[4][4];
        float TextureMatrix[3][4];
        float MaskColor[4];
    };

    Microsoft::WRL::ComPtr<ID3D11VertexShader>  spriteVS = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   spritePS = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        spriteCB = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   spriteLayout = nullptr;
    IMeshDataPtr                                spriteMesh = nullptr;
};

}   //end of ShiftEngine namespace
