#include "D3D11Context.h"

using namespace ShiftEngine;

D3D11Context::~D3D11Context()
{
    if (DeviceContext)
        DeviceContext->ClearState();

    // make sure here that objects are released in appropriate order
    // to avoid crashes and asserts in DX runtime
    bsAlpha.Release();
    bsAdditive.Release();
    bsNormal.Release();

    rsNormal.Release();
    rsWireframe.Release();
    rsNoCulling.Release();

    dsStateZOn.Release();
    dsStateZOff.Release();

    DefaultDS.reset(nullptr);
    DefaultRT.reset(nullptr);
    DeviceContext.Release();
    SwapChain.Release();
    Device.Release();
}

void D3D11Context::ClearDefaultRenderTarget()
{
    static float clearColors[] = {0.0f, 0.0f, 0.0f, 1.0f};
    DeviceContext->ClearRenderTargetView(DefaultRT->view, clearColors);
    DeviceContext->ClearDepthStencilView(DefaultDS->view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

HRESULT D3D11Context::CreateStates()
{
    D3D11_BLEND_DESC bdDescNormal;
    ZeroMemory(&bdDescNormal, sizeof(D3D11_BLEND_DESC));
    for (int i = 0; i < 8; ++i)
    {
        bdDescNormal.RenderTarget[i].BlendOp = D3D11_BLEND_OP_MAX;

        bdDescNormal.RenderTarget[i].DestBlend = D3D11_BLEND_DEST_COLOR;

        bdDescNormal.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_COLOR;

        bdDescNormal.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    HRESULT hr = Device->CreateBlendState(&bdDescNormal, &bsNormal);
    if (FAILED(hr))
        return hr;

    D3D11_BLEND_DESC bdDescAlpha;
    ZeroMemory(&bdDescAlpha, sizeof(D3D11_BLEND_DESC));
    bdDescAlpha.AlphaToCoverageEnable = false;
    for (int i = 0; i < 8; ++i)
    {
        bdDescAlpha.RenderTarget[i].BlendEnable = true;

        bdDescAlpha.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        bdDescAlpha.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        bdDescAlpha.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        bdDescAlpha.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
        
        bdDescAlpha.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        bdDescAlpha.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;

        bdDescAlpha.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    hr = Device->CreateBlendState(&bdDescAlpha, &bsAlpha);
    if (FAILED(hr))
        return hr;

    D3D11_BLEND_DESC bdDescAdditive;
    ZeroMemory(&bdDescAdditive, sizeof(D3D11_BLEND_DESC));
    bdDescAdditive.AlphaToCoverageEnable = false;
    for (int i = 0; i < 8; ++i)
    {
        bdDescAdditive.RenderTarget[i].BlendEnable = true;

        bdDescAdditive.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        bdDescAdditive.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        bdDescAdditive.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
        bdDescAdditive.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;

        bdDescAdditive.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        bdDescAdditive.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;

        bdDescAdditive.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
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
    HRESULT hr = DeviceContext->QueryInterface(&UserAnnotationsHandler);
    if (FAILED(hr))
        return hr;
}
