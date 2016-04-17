#pragma once

#include <string>

#include "IMeshData.h"
#include "ITexture.h"
#include "IProgram.h"
#include "VertexTypes.h"

#include <MathLib/math.h>

namespace ShiftEngine
{
    //2D sprite
    class Sprite
    {
    public:
        Sprite();
        Sprite(const std::wstring & filename,
            const MathLib::vec2f & texCoordLeftTop = { 0.0f, 0.0f },
            const MathLib::vec2f & texCoordRightBottom = { 1.0f, 1.0f });
        ~Sprite();

        void SetPosition(const MathLib::vec2f & pos);
        void SetRotation(float rot);
        void SetScale(const MathLib::vec2f & sc);
        void SetTexture(ShiftEngine::ITexturePtr ptr);
        void SetTexture(const std::wstring & filename);
        void SetMaskColor(const MathLib::vec4f & color);

        void SetSizeInPixels(int x, int y);
        void Draw();

        MathLib::vec2f GetTextureDimensions() const;

    private:
        void CreateBuffers(const MathLib::vec2f & LT, const MathLib::vec2f & RB);
        void LoadShader();

        ShiftEngine::ITexturePtr texture = nullptr;
        ShiftEngine::IMeshDataPtr spriteMesh = nullptr;

        MathLib::vec4f MaskColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        MathLib::vec2f Position = { 0.0f, 0.0f };
        float Rotation = 0.0f;
        MathLib::vec2f Scale = { 1.0f, 1.0f };

        static ShiftEngine::IProgramPtr SpriteShader;
    };
}
