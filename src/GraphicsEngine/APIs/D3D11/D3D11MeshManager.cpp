#include "D3D11MeshManager.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include <ShiftEngine.h>

using namespace ShiftEngine;

D3D11MeshManager::D3D11MeshManager(Microsoft::WRL::ComPtr<ID3D11Device> device)
    : pDevice(device)
{
}

IMeshDataPtr D3D11MeshManager::CreateMeshFromVertices(const uint8_t * verticesData,
                                                      size_t verticesDataSize,
                                                      const std::vector<uint32_t> & indicesData,
                                                      VertexType type,
                                                      const MathLib::AABB & bbox)
{
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediate = nullptr;
    pDevice->GetImmediateContext(&pImmediate);
    D3D11MeshDataPtr out = std::make_shared<D3D11MeshData>(nullptr, nullptr, pDevice, pImmediate);
    if (!out->CreateBuffers(false, verticesData, verticesDataSize, indicesData.data(), indicesData.size() * sizeof(uint32_t), type, bbox))
    {
        LOG_ERROR("Unable to create mesh from vertices");
        return nullptr;
    }
    return out;
}
