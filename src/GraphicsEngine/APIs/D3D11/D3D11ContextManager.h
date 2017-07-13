#pragma once

#include <exception>
#include <vector>

#include "D3D11Context.h"
#include "D3D11TextureManager.h"
#include "D3D11RenderTarget.h"

#include "../../IContextManager.h"
#include "../../MiscTypes.h"
// #include "../../FontManager.h"

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
    ~D3D11ContextManager() override = default;

    bool                                Initialize(GraphicEngineSettings _Settings, PathSettings _Paths) override;
    std::wstring                        GetGPUDescription() override;
    ITexturePtr                         LoadTexture(const std::wstring & FileName) override;
    ITextureManager *                   GetTextureManager() override;
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
    D3D11TextureManager *                       textureManager = nullptr;

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

    struct SpriteVertex
    {
        float position[2];
        float texcoord[2];
    };

    Microsoft::WRL::ComPtr<ID3D11VertexShader>  spriteVS = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   spritePS = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        spriteCB = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        spriteGeometry = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   spriteLayout = nullptr;
};

}   //end of ShiftEngine namespace
