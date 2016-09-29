#pragma once

#include <d3d11_1.h>
#include <atlbase.h>

#include "D3D11DepthStencil.h"
#include "D3D11RenderTarget.h"

namespace ShiftEngine
{

struct D3D11Context
{
    ~D3D11Context();
    void ClearDefaultRenderTarget();
    HRESULT CreateStates();
    HRESULT CreateAnnotationsHandler();

    CComPtr<ID3D11Device>               Device = nullptr;
    CComPtr<ID3D11DeviceContext>        DeviceContext = nullptr;
    CComPtr<IDXGISwapChain>             SwapChain = nullptr;
    CComPtr<ID3DUserDefinedAnnotation>  UserAnnotationsHandler = nullptr;

    std::unique_ptr<D3D11RenderTarget>  DefaultRT = std::make_unique<D3D11RenderTarget>(nullptr, nullptr);
    std::unique_ptr<D3D11DepthStencil>  DefaultDS = std::make_unique<D3D11DepthStencil>(nullptr, nullptr);

    CComPtr<ID3D11DepthStencilState>    dsStateZOn = nullptr;
    CComPtr<ID3D11DepthStencilState>    dsStateZOff = nullptr;

    CComPtr<ID3D11RasterizerState>      rsWireframe = nullptr;
    CComPtr<ID3D11RasterizerState>      rsNormal = nullptr;
    CComPtr<ID3D11RasterizerState>      rsNoCulling = nullptr;

    CComPtr<ID3D11BlendState>           bsAlpha = nullptr;
    CComPtr<ID3D11BlendState>           bsNormal = nullptr;
    CComPtr<ID3D11BlendState>           bsAdditive = nullptr;

    D3D11Context() = default;
    D3D11Context(const D3D11Context & ref) = delete;
    D3D11Context(D3D11Context && ref) = delete;
    D3D11Context & operator=(const D3D11Context & ref) = delete;
    D3D11Context & operator=(D3D11Context && ref) = delete;
};

}
