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

class D3D11ContextManager : public IContextManager
{
public:
    D3D11ContextManager(HWND hwnd);
    ~D3D11ContextManager();

    bool                                Initialize(GraphicEngineSettings _Settings, PathSettings _Paths) override;
    std::wstring                        GetGPUDescription() override;
    ITexturePtr                         LoadTexture(const std::wstring & FileName) override;
    ITextureManager *                   GetTextureManager() override;
    // FontManager*                        GetFontManager() override;
    const GraphicEngineSettings &       GetEngineSettings() const override;
    const PathSettings &                GetPaths() const override;
    void                                DrawAll(RenderQueue& queue, double dt) override;

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
    int                                 DrawMesh();
    ID3D11Device*                       GetDevicePtr() const;

    HWND                                windowHandle;
    PathSettings                        enginePaths;
    GraphicEngineSettings               engineSettings;

    D3D11Context                        graphicsContext;
    // FontManager*                        fontManager = nullptr;
    D3D11TextureManager *               textureManager = nullptr;

    RasterizerState                     currentRasterizerState = RasterizerState::Normal;
    BlendingState                       currentBlendingState = BlendingState::None;

    bool                                zBufferState = true;
    bool                                cullingEnabled = true;
};

}   //end of ShiftEngine namespace
