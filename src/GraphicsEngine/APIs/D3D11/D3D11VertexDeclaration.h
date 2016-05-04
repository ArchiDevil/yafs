#pragma once

#include <d3d11.h>
#include <memory>

#include "../../IVertexDeclaration.h"

namespace ShiftEngine
{

class D3D11VertexDeclaration : public IVertexDeclaration
{
public:
    D3D11VertexDeclaration(CComPtr<ID3D11InputLayout> IL = nullptr, CComPtr<ID3D11DeviceContext> pDeviceContext = nullptr)
        : IL(IL)
        , pDeviceContext(pDeviceContext)
    {}

    void Bind() override
    {
        if (IL)
            pDeviceContext->IASetInputLayout(this->IL);
    }

    CComPtr<ID3D11InputLayout> IL = nullptr;
    CComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
};

typedef std::shared_ptr<D3D11VertexDeclaration> D3D11VertexDeclarationPtr;

}
