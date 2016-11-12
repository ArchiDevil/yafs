#pragma once

#include <D3D11.h>
#include <wrl.h>

#include <memory>

namespace ShiftEngine
{

struct D3D11RenderTarget
{
    D3D11RenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _rt, 
                      Microsoft::WRL::ComPtr<ID3D11Texture2D> _tex)
        : view(_rt)
        , texture(_tex)
    {}

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
};

using D3D11RenderTargetPtr = std::shared_ptr<D3D11RenderTarget>;

}
