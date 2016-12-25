#pragma once

#include <D3D11.h>
#include <wrl.h>
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
    D3D11Texture(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext,
                 size_t width,
                 size_t height,
                 TextureType type = TextureType::Texture2D,
                 Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr);
    void Bind(unsigned int index, BindingPoint point) override;

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
};

using D3D11TexturePtr = std::shared_ptr<D3D11Texture>;

}
