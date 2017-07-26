#include "D3D11Mesh.h"

#include <cassert>

using namespace ShiftEngine;

D3D11MeshData::D3D11MeshData(Microsoft::WRL::ComPtr<ID3D11Buffer> _VB /*= nullptr*/,
                             Microsoft::WRL::ComPtr<ID3D11Buffer> _IB /*= nullptr*/,
                             Microsoft::WRL::ComPtr<ID3D11Device> pDevice /*= nullptr*/,
                             Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext /*= nullptr*/)
    : VertexBuffer(_VB)
    , IndexBuffer(_IB)
    , pDevice(pDevice)
    , pDeviceContext(pDeviceContext)
{
}

bool D3D11MeshData::CreateBuffers(bool dynamic,
                                  const uint8_t * vData,
                                  size_t vDataSize,
                                  const uint32_t * iData,
                                  size_t iDataSize,
                                  VertexType type,
                                  const MathLib::AABB & _bbox)
{
    assert(pDevice);
    assert(pDeviceContext);

    if (!vData || !vDataSize)
        return false;

    D3D11_BUFFER_DESC vBuffDesc;
    ZeroMemory(&vBuffDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11_BUFFER_DESC iBuffDesc;
    ZeroMemory(&iBuffDesc, sizeof(D3D11_BUFFER_DESC));

    vBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vBuffDesc.ByteWidth = (UINT)vDataSize;
    vBuffDesc.MiscFlags = NULL;

    if (dynamic)
    {
        vBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else
    {
        vBuffDesc.CPUAccessFlags = NULL;
        vBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    }

    if (vData)
    {
        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
        data.pSysMem = vData;

        if (FAILED(pDevice->CreateBuffer(&vBuffDesc, &data, &VertexBuffer)))
            return false;
    }

    iBuffDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    iBuffDesc.ByteWidth = (UINT)iDataSize;
    iBuffDesc.MiscFlags = NULL;

    if (dynamic)
    {
        iBuffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        iBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else
    {
        iBuffDesc.CPUAccessFlags = NULL;
        iBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    }

    if (iData)
    {
        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
        data.pSysMem = iData;

        if (FAILED(pDevice->CreateBuffer(&iBuffDesc, &data, &IndexBuffer)))
            return false;
    }

    vertexSize = GetVertexSizeForVertexType(type);
    verticesCount = vDataSize / vertexSize;
    indicesCount = iDataSize / sizeof(uint32_t);
    bbox = _bbox;

    return true;
}

size_t D3D11MeshData::Draw()
{
    if (!pDevice)
        return 0;

    if (!vertexSize || !verticesCount)
        return 0;

    unsigned int stride = (unsigned int)vertexSize;
    unsigned int offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
    if (indicesCount > 0)
    {
        pDeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        pDeviceContext->DrawIndexed((UINT)indicesCount, 0, 0);
        return indicesCount / 3;
    }
    else
    {
        pDeviceContext->Draw((UINT)verticesCount, 0);
        return verticesCount;
    }
}
