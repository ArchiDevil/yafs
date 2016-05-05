#pragma once

#include <d3d11.h>
#include <atlbase.h>
#include <string>
#include <memory>

#include "D3D11VertexDeclaration.h"

#include "../../IMeshData.h"
#include "../../VertexTypes.h"
#include "../../MiscTypes.h"

namespace ShiftEngine
{

class D3D11MeshData : public IMeshData
{
public:
    D3D11MeshData(ID3D11Buffer * _VB = nullptr, ID3D11Buffer * _IB = nullptr,
                  ID3D11Device * pDevice = nullptr, ID3D11DeviceContext * pDeviceContext = nullptr);

    bool CreateBuffers(bool dynamic,
                       const uint8_t * vData,
                       size_t vDataSize,
                       const uint32_t * iData,
                       size_t iDataSize,
                       const VertexSemantic * semantic,
                       const IVertexDeclarationPtr & declaration,
                       const MathLib::AABB & bbox) override;
    size_t Draw() override;
    void Clear() override;

private:
    CComPtr<ID3D11Device> pDevice = nullptr;
    CComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
    CComPtr<ID3D11Buffer> VertexBuffer = nullptr;
    CComPtr<ID3D11Buffer> IndexBuffer = nullptr;
};

typedef std::shared_ptr<D3D11MeshData> D3D11MeshDataPtr;

}
