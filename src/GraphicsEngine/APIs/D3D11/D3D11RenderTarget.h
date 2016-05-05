#pragma once

#include <D3D11.h>
#include <atlbase.h>

#include <memory>

namespace ShiftEngine
{

struct D3D11RenderTarget
{
    D3D11RenderTarget(CComPtr<ID3D11RenderTargetView> _rt, CComPtr<ID3D11Texture2D> _tex)
        : view(_rt)
        , texture(_tex)
    {}

    CComPtr<ID3D11RenderTargetView> view = nullptr;
    CComPtr<ID3D11Texture2D> texture = nullptr;
};

typedef std::shared_ptr<D3D11RenderTarget> D3D11RenderTargetPtr;

}
