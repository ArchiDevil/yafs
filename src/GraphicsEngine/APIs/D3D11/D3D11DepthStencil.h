#pragma once

#include <D3D11.h>
#include <atlbase.h>

#include <memory>

namespace ShiftEngine
{

struct D3D11DepthStencil
{
    D3D11DepthStencil(CComPtr<ID3D11DepthStencilView> _rt, CComPtr<ID3D11Texture2D> _tex)
        : view(_rt)
        , texture(_tex)
    {}

    CComPtr<ID3D11DepthStencilView> view = nullptr;
    CComPtr<ID3D11Texture2D> texture = nullptr;
};

using D3D11DepthStencilPtr = std::shared_ptr<D3D11DepthStencil>;

}
