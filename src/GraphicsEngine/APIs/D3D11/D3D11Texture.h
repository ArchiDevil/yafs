#pragma once

#include <D3D11.h>
#include <atlbase.h>
#include <memory>

#include "../../ITexture.h"
#include "../../MiscTypes.h"

namespace ShiftEngine
{

class D3D11Texture : public ITexture
{
    friend class Renderer;

    friend class D3D11ContextManager;
    friend class D3D11TextureManager;
    friend class D3D11Program;

public:
    D3D11Texture(CComPtr<ID3D11DeviceContext> pDeviceContext, size_t width, size_t height, TextureType type = TextureType::Texture2D, CComPtr<ID3D11ShaderResourceView> texture = nullptr);
    void Bind(unsigned int index, BindingPoint point) override;

private:
    CComPtr<ID3D11ShaderResourceView> texture = nullptr;
    CComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
};

typedef std::shared_ptr<D3D11Texture> D3D11TexturePtr;

}
