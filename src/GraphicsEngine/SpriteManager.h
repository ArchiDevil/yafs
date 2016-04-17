#pragma once

#include <Utilities/IManager.h>

#include "Sprite.h"

#include <memory>

namespace ShiftEngine
{

class SpriteManager final : public IManager
{
public:
    std::shared_ptr<Sprite> CreateSprite(const std::wstring & textureName);
    void DrawSprites();

private:
    void CreateSpriteProgram();
    void CreateSpriteMesh();
    ITexturePtr CreateSpriteTexture(const std::wstring & filename);

    std::vector<std::shared_ptr<Sprite>> sprites;
    IProgramPtr spriteProgram = nullptr;
    IMeshDataPtr spriteMesh = nullptr;

};

}
