#pragma once

#include "IMeshData.h"
#include "ITexture.h"
#include "IProgram.h"
#include "VertexTypes.h"

#include <MathLib/math.h>

namespace ShiftEngine
{

//2D sprite
class Sprite final
{
public:
    Sprite();
    Sprite(ITexturePtr spriteTexture,
           const MathLib::vec2f & texCoordLeftTop = {0.0f, 0.0f},
           const MathLib::vec2f & texCoordRightBottom = {1.0f, 1.0f});

    Sprite(const Sprite&) = delete;
    Sprite(Sprite&&) = delete;
    Sprite& operator=(const Sprite&) = delete;
    Sprite& operator=(Sprite&&) = delete;

    void SetPosition(const MathLib::vec2f & pos);
    void SetRotation(float rot);
    void SetScale(const MathLib::vec2f & sc);
    void SetTexture(ITexturePtr ptr);
    void SetMaskColor(const MathLib::vec4f & color);

    void SetSizeInPixels(int x, int y);

    MathLib::vec2f GetTextureDimensions() const;
    const ITexturePtr & GetTexture() const;
    const MathLib::vec2f & GetPosition() const;
    const MathLib::vec2f & GetScale() const;
    float GetRotation() const;
    const MathLib::vec4f & GetMaskColor() const;

private:
    ITexturePtr spriteTexture = nullptr;
    MathLib::vec4f maskColor = {1.0f, 1.0f, 1.0f, 1.0f};
    MathLib::vec2f position = {0.0f, 0.0f};
    MathLib::vec2f scale = {1.0f, 1.0f};
    float rotation = 0.0f;
};

}
