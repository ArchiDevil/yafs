#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <memory>

#include "../../IVertexDeclaration.h"

namespace ShiftEngine
{

class D3D11VertexDeclaration : public IVertexDeclaration
{
public:
    D3D11VertexDeclaration(Microsoft::WRL::ComPtr<ID3D11InputLayout> IL = nullptr,
                           Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr)
        : IL(IL)
        , pDeviceContext(pDeviceContext)
    {
    }

    void Bind() override
    {
        if (IL)
            pDeviceContext->IASetInputLayout(IL.Get());
    }

    Microsoft::WRL::ComPtr<ID3D11InputLayout> IL = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
};

using D3D11VertexDeclarationPtr = std::shared_ptr<D3D11VertexDeclaration>;

}
