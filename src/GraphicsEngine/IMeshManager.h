#pragma once

#include "IMeshData.h"

#include <vector>

namespace ShiftEngine
{
class IMeshManager
{
public:
    virtual ~IMeshManager() = default;

    virtual IMeshDataPtr CreateMeshFromVertices(const uint8_t* verticesData,
                                                size_t verticesDataSize,
                                                const std::vector<uint32_t>& indicesData,
                                                VertexType type,
                                                const MathLib::AABB & bbox) = 0;

    template<typename vertex_t>
    IMeshDataPtr CreateMeshFromVertices(const std::vector<vertex_t>& verticesData,
                                        const std::vector<uint32_t>& indicesData,
                                        const MathLib::AABB& bbox)
    {
        return CreateMeshFromVertices((const uint8_t*)verticesData.data(), 
                                      verticesData.size() * sizeof(vertex_t), 
                                      indicesData, 
                                      vertex_type_match<vertex_t>::value, 
                                      bbox);
    }
};
}
