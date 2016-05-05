#pragma once

#include <d3d11.h>

#include "D3D11RenderTarget.h"

struct D3D11Context
{
    ID3D11Device *                      Device = nullptr;
    ID3D11DeviceContext *               DeviceContext = nullptr;
    IDXGISwapChain *                    SwapChain = nullptr;
    std::unique_ptr<D3D11RenderTarget>  DefaultRT = nullptr;
    ID3D11Texture2D *                   DepthStencilBuffer = nullptr;
    ID3D11DepthStencilView *            DepthStencilView = nullptr;

    ID3D11DepthStencilState *           dsStateZOn = nullptr;
    ID3D11DepthStencilState *           dsStateZOff = nullptr;

    ID3D11RasterizerState *             rsWireframe = nullptr;
    ID3D11RasterizerState *             rsNormal = nullptr;
    ID3D11RasterizerState *             rsNoCulling = nullptr;

    ID3D11BlendState *                  bsAlpha = nullptr;
    ID3D11BlendState *                  bsNormal = nullptr;
    ID3D11BlendState *                  bsAdditive = nullptr;

    D3D11Context(const D3D11Context & ref) = delete;
    D3D11Context(D3D11Context && ref) = delete;
    D3D11Context & operator=(const D3D11Context & ref) = delete;
    D3D11Context & operator=(D3D11Context && ref) = delete;

    D3D11Context();
    ~D3D11Context();
    HRESULT CreateStates();
};
