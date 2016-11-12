#pragma once

#include <memory>

#include "MiscTypes.h"

namespace ShiftEngine
{
class IProgram;

enum class BindingPoint
{
    Vertex,
    Pixel,
    Geometry,
    Hull,
    Domain,
    Compute
};

class ITexture
{
public:
    ITexture(size_t width, size_t height, TextureType type = TextureType::Texture2D)
        : width(width)
        , height(height)
        , type(type)
    {
    }

    virtual ~ITexture() = default;

    int GetHeight() const
    {
        return height;
    }

    int GetWidth() const
    {
        return width;
    }

    TextureType GetType() const
    {
        return type;
    }

    virtual void Bind(unsigned int index, BindingPoint point) = 0;

private:
    int width;
    int height;
    TextureType type;
};

using ITexturePtr = std::shared_ptr<ITexture>;
}
