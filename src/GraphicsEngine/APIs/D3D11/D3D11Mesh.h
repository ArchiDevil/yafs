#pragma once

#include <d3d11.h>
#include <wrl.h>
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
    D3D11MeshData(Microsoft::WRL::ComPtr<ID3D11Buffer> _VB = nullptr,
                  Microsoft::WRL::ComPtr<ID3D11Buffer> _IB = nullptr,
                  Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr,
                  Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr);

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
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;
};

using D3D11MeshDataPtr = std::shared_ptr<D3D11MeshData>;

}
