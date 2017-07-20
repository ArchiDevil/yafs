#pragma once

#include <d3d11_1.h>
#include <wrl.h>

#include "D3D11DepthStencil.h"
#include "D3D11RenderTarget.h"

#include "../../MiscTypes.h"

namespace ShiftEngine
{

struct D3D11Context
{
    D3D11Context(HWND windowHandle, const GraphicEngineSettings &settings);

    D3D11Context(const D3D11Context & ref) = delete;
    D3D11Context(D3D11Context && ref) = delete;
    D3D11Context & operator=(const D3D11Context & ref) = delete;
    D3D11Context & operator=(D3D11Context && ref) = delete;

    ~D3D11Context();

    void ClearDefaultRenderTarget();
    HRESULT CreateStates();
    HRESULT CreateAnnotationsHandler();

    Microsoft::WRL::ComPtr<ID3D11Device>                Device = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>         DeviceContext = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain>              SwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>   UserAnnotationsHandler = nullptr;

    std::unique_ptr<D3D11RenderTarget>                  DefaultRT = std::make_unique<D3D11RenderTarget>(nullptr, nullptr);
    std::unique_ptr<D3D11DepthStencil>                  DefaultDS = std::make_unique<D3D11DepthStencil>(nullptr, nullptr);

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>     dsStateZOn = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>     dsStateZOff = nullptr;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState>       rsWireframe = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>       rsNormal = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>       rsNoCulling = nullptr;

    Microsoft::WRL::ComPtr<ID3D11BlendState>            bsAlpha = nullptr;
    Microsoft::WRL::ComPtr<ID3D11BlendState>            bsNormal = nullptr;
    Microsoft::WRL::ComPtr<ID3D11BlendState>            bsAdditive = nullptr;

};

}
