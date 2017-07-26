#pragma once

#include "MiscTypes.h"

#include <MathLib/math.h>

#include <string>
#include <memory>
#include <type_traits>

namespace ShiftEngine
{
enum class VertexType
{
    SpriteVertexType
};

struct SpriteVertex
{
    float position[2];
    float texcoord[2];
};

inline size_t GetVertexSizeForVertexType(VertexType type)
{
    switch (type)
    {
    case ShiftEngine::VertexType::SpriteVertexType:
        return sizeof(SpriteVertex);
    default:
        return 0;
    }
}

template<typename vertex_t>
struct vertex_type_match : std::enable_if<false>
{
};

template<>
struct vertex_type_match<SpriteVertex>
{
    static constexpr VertexType value = VertexType::SpriteVertexType;
};

class IMeshData
{
public:
    virtual ~IMeshData() = default;

    virtual size_t Draw() = 0;

    size_t GetVerticesCount() const
    {
        return verticesCount;
    }

    size_t GetIndicesCount() const
    {
        return indicesCount;
    }

    size_t GetVertexSize() const
    {
        return vertexSize;
    }

    MathLib::AABB GetBBox() const
    {
        return bbox;
    }

    virtual bool CreateBuffers(bool dynamic,
                               const uint8_t * vData,
                               size_t vDataSize,
                               const uint32_t * iData,
                               size_t iDataSize,
                               VertexType vertexType,
                               const MathLib::AABB & bbox) = 0;

    IMeshData() = default;
    IMeshData(IMeshData & other) = default;
    IMeshData & operator=(IMeshData & other) = default;

protected:
    size_t verticesCount = 0;
    size_t indicesCount = 0;
    size_t vertexSize = 0;
    MathLib::AABB bbox = {};
};

using IMeshDataPtr = std::shared_ptr<IMeshData>;

}
