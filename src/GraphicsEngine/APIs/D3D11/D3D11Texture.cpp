#include "D3D11Texture.h"

#include <cassert>

using namespace ShiftEngine;

D3D11Texture::D3D11Texture(CComPtr<ID3D11DeviceContext> pDeviceContext, size_t width, size_t height, TextureType type, CComPtr<ID3D11ShaderResourceView> texture)
    : ITexture(width, height, type)
    , texture(texture)
    , pDeviceContext(pDeviceContext)
{
}

void D3D11Texture::Bind(unsigned int index, BindingPoint point)
{
    switch (point)
    {
    case BindingPoint::Vertex:
        pDeviceContext->VSSetShaderResources(index, 1, &texture.p);
        break;
    case BindingPoint::Pixel:
        pDeviceContext->PSSetShaderResources(index, 1, &texture.p);
        break;
    case BindingPoint::Geometry:
        pDeviceContext->GSSetShaderResources(index, 1, &texture.p);
        break;
    case BindingPoint::Hull:
        pDeviceContext->HSSetShaderResources(index, 1, &texture.p);
        break;
    case BindingPoint::Domain:
        pDeviceContext->DSSetShaderResources(index, 1, &texture.p);
        break;
    case BindingPoint::Compute:
        pDeviceContext->CSSetShaderResources(index, 1, &texture.p);
        break;
    default:
        assert(false);
        break;
    }
}
