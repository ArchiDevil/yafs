#include "BackgroundManager.h"

#include "Game.h"

using namespace ShiftEngine;

BackgroundManager::BackgroundManager(EntityManager *entityMgr)
{
    auto* sceneGraph = GetSceneGraph();
    for (int i = 1; i < 4; ++i)
    {
        layers.push_back(sceneGraph->AddEmptyNode());
        //FIXME: this sprite image only for example, needed to be fixed
        SpriteSceneNode *sprite = sceneGraph->AddSpriteNode(L"sprite.png", {{}, {}}, {1.0, 1.0}, i);
        sprite->SetLocalScale((4 - i) * 0.5f);
        sprite->SetMaskColor({1.0f / i, 1.0f / i, 1.0f / i, 1.0f});
        layers.back()->AddChild(sprite);
        entityMgr->CreateBackgroundEntity(sprite, i);
    }
}

void BackgroundManager::Update(double dt)
{
    static double time = 0.0;
    time += dt;
    float sin_val = sinf((float)time) * 0.7f;
    float cos_val = cosf((float)time) * 0.7f;

    // need to take player position
    MathLib::vec2f playerPosition = GoingHome::GetGamePtr()->GetPlayerPtr()->GetPosition();
    for (int i = 1; i < 4; ++i)
    {
        // invert position of background layer simulating movement to the different direction of player
        float k = 1.0f / i;
        layers[i - 1]->SetLocalPosition({/*playerPosition.x*/sin_val * k, /*playerPosition.y*/cos_val * k, 0.0f});
    }
}
