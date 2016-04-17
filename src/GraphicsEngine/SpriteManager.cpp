#include "SpriteManager.h"

#include "ShiftEngine.h"

#include <array>

using namespace ShiftEngine;
using namespace MathLib;

std::shared_ptr<Sprite> SpriteManager::CreateSprite(const std::wstring & textureName)
{
    if (!spriteProgram)
        CreateSpriteProgram();

    if (!spriteMesh)
        CreateSpriteMesh();

    auto & texture = CreateSpriteTexture(textureName);
    sprites.push_back(std::make_unique<Sprite>(texture));
    return sprites.back();
}

void SpriteManager::DrawSprites()
{
    auto settings = GetContextManager()->GetEngineSettings();

    for(auto & sprite : sprites)
    {
        if (sprite.use_count() == 1)
        {
            std::swap(sprite, sprites.back());
            sprites.pop_back();
        }

        auto & texture = sprite->GetTexture();

        if (!texture)
            return;

        const vec2f & position = sprite->GetPosition();
        const vec2f & scale = sprite->GetScale();
        float rotation = sprite->GetRotation();
        const vec4f & maskColor = sprite->GetMaskColor();

        mat4f matResult, matScale, matPos, matRot;
        matPos = matrixTranslation(position.x, position.y, 0.0f);
        matScale = matrixScaling(scale.x, scale.y, 0.0f);
        matRot = matrixRotationZ(rotation);
        matResult = matrixOrthoOffCenterLH<float>(0.0, (float)settings.screenWidth, (float)settings.screenHeight, 0.0f, 0.0f, 1.0f);
        matResult = (matScale * matRot * matPos) * matResult;

        spriteProgram->SetMatrixConstantByName("matRes", (float*)matResult);
        spriteProgram->SetTextureByName("Texture", texture);
        spriteProgram->SetVectorConstantByName("MaskColor", maskColor.ptr());
        spriteProgram->Apply(true);
        GetContextManager()->DrawMesh(spriteMesh);
    }
}

void SpriteManager::CreateSpriteProgram()
{
    spriteProgram = GetContextManager()->LoadShader(L"SpriteShader.fx");
}

void SpriteManager::CreateSpriteMesh()
{
    std::array<PlainSpriteVertex, 4> ver = {};
    ver[0] = {{-0.5f, -0.5f}, {0.0f, 0.0f}};
    ver[1] = {{0.5f, -0.5f}, {1.0f, 0.0f}};
    ver[2] = {{-0.5f, 0.5f}, {0.0f, 1.0f}};
    ver[3] = {{0.5f, 0.5f}, {1.0f, 1.0f}};

    std::vector<uint32_t> ind = {0, 1, 2, 1, 3, 2};

    IMeshManager * pMeshManager = GetContextManager()->GetMeshManager();
    spriteMesh = pMeshManager->CreateMeshFromVertices((uint8_t*)ver.data(),
                                                      ver.size() * sizeof(PlainSpriteVertex),
                                                      ind,
                                                      &plainSpriteVertexSemantic,
                                                      {});
}

ITexturePtr SpriteManager::CreateSpriteTexture(const std::wstring & filename)
{
    ITexturePtr texture = nullptr;
    if (!filename.empty())
        texture = GetContextManager()->LoadTexture(filename);
    return texture;
}
