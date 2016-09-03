#include "BackgroundManager.h"

#include "Game.h"

using namespace ShiftEngine;

BackgroundManager::BackgroundManager(EntityManager *entityMgr)
{
    auto* sceneGraph = GetSceneGraph();
    for (int i = 1; i < 4; ++i)
        layers.push_back(sceneGraph->AddEmptyNode());

    for (int i = 0; i < 50; ++i)
    {
        SpriteSceneNode *sprite = sceneGraph->AddSpriteNode(L"cloud.png", {{}, {}}, {1.0, 1.0}, 3);
        // sprite->SetDrawingMode(SpriteSceneNode::SpriteDrawingMode::Additive);
        sprite->SetLocalScale(std::rand() % 330 / 100.0f + 0.7f);
        sprite->SetLocalPosition({float(std::rand() % 800) / 100.0f - 4.0f, float(std::rand() % 600) / 100.0f - 3.0f, 0.0f});
        layers[2]->AddChild(sprite);
        entityMgr->CreateBackgroundBlinker(sprite);
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
