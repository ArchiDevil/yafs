#include "Sprite.h"

#include "ShiftEngine.h"

using namespace MathLib;
using namespace ShiftEngine;

Sprite::Sprite()
    : Sprite {nullptr}
{}

Sprite::Sprite(ITexturePtr spriteTexture, const vec2f & texCoordLeftTop, const vec2f & texCoordRightBottom)
    : spriteTexture(spriteTexture)
{}

void Sprite::SetTexture(ShiftEngine::ITexturePtr ptr)
{
    spriteTexture = ptr;

    if (ptr)
        SetScale({1.0f, 1.0f});
}

void Sprite::SetPosition(const vec2f & pos)
{
    position = pos;
}

void Sprite::SetRotation(float rot)
{
    rotation = rot;
}

void Sprite::SetScale(const vec2f & sc)
{
    scale = sc;

    if (!spriteTexture)
        return;

    scale.x *= spriteTexture->GetWidth();
    scale.y *= spriteTexture->GetHeight();
}

vec2f Sprite::GetTextureDimensions() const
{
    if (!spriteTexture)
        return {0.0f, 0.0f};

    return {(float)spriteTexture->GetWidth(), (float)spriteTexture->GetHeight()};
}

const ITexturePtr & Sprite::GetTexture() const
{
    return spriteTexture;
}

void Sprite::SetMaskColor(const vec4f & color)
{
    maskColor = color;
}

void Sprite::SetSizeInPixels(int x, int y)
{
    if (!spriteTexture)
        SetScale({(float)x / spriteTexture->GetWidth(), (float)y / spriteTexture->GetHeight()});
}

const MathLib::vec2f & ShiftEngine::Sprite::GetPosition() const
{
    return position;
}

const MathLib::vec2f & ShiftEngine::Sprite::GetScale() const
{
    return scale;
}

float ShiftEngine::Sprite::GetRotation() const
{
    return rotation;
}

const MathLib::vec4f & ShiftEngine::Sprite::GetMaskColor() const
{
    return maskColor;
}
