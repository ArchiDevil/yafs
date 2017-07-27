#pragma once

#include <IMeshManager.h>
#include "D3D11Mesh.h"

#include <D3D11.h>
#include <wrl.h>

#include <string>
#include <map>
#include <set>

namespace ShiftEngine
{

class D3D11MeshManager : public IMeshManager
{
public:
    D3D11MeshManager(Microsoft::WRL::ComPtr<ID3D11Device> device);
    
    IMeshDataPtr CreateMeshFromVertices(const uint8_t * verticesData,
                                        size_t verticesDataSize,
                                        const std::vector<uint32_t> & indicesData,
                                        VertexType type,
                                        const MathLib::aabb3f & bbox) override;

    using IMeshManager::CreateMeshFromVertices;

private:
    IMeshDataPtr errorMesh;
    std::wstring errorName;
    std::map<std::wstring, D3D11MeshDataPtr> meshesData;
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
};

}
