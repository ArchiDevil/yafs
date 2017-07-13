#pragma once

#include <string>
#include <memory>

#include "MiscTypes.h"

namespace ShiftEngine
{

class IMeshData
{
public:
    virtual ~IMeshData() = default;

    virtual size_t Draw() = 0;
    virtual void Clear() = 0;

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
                               const MathLib::AABB & bbox,
                               size_t vertexSize) = 0;

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
