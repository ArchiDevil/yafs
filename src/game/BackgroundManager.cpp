#include "BackgroundManager.h"

#include "Game.h"

using namespace ShiftEngine;

BackgroundManager::BackgroundManager(EntityManager *entityMgr)
{
    auto* const sceneGraph = GetSceneGraph();
    for (int i = 0; i < 3; ++i)
        backgroundLayers.push_back(sceneGraph->AddEmptyNode());

    for (int i = 0; i < 50; ++i)
    {
        SpriteSceneNode* const sprite = sceneGraph->AddSpriteNode(L"cloud.png", 3);
        sprite->SetDrawingMode(SpriteSceneNode::SpriteDrawingMode::Additive);
        sprite->SetLocalScale(std::rand() % 330 / 100.0f + 0.7f);
        sprite->SetLocalPosition({float(std::rand() % 800) / 100.0f - 4.0f, float(std::rand() % 600) / 100.0f - 3.0f, 0.0f});
        backgroundLayers[2]->AddChild(sprite);
        entityMgr->CreateBackgroundBlinker(sprite);
    }

    for (int i = 0; i < 25; ++i)
    {
        SpriteSceneNode* const sprite = sceneGraph->AddSpriteNode(L"enemy_sprite.png", 2);
        sprite->SetDrawingMode(SpriteSceneNode::SpriteDrawingMode::Additive);
        sprite->SetLocalPosition({float(std::rand() % 800) / 100.0f - 4.0f, float(std::rand() % 600) / 100.0f - 3.0f, 0.0f});
        sprite->SetLocalScale(0.3f);
        backgroundLayers[1]->AddChild(sprite);
        entityMgr->CreateBackgroundBlinker(sprite)->SetBlinkPeriod(0.1f);
    }

    for (int i = 0; i < 5; ++i)
    {
        SpriteSceneNode* const sprite = sceneGraph->AddSpriteNode(L"enemy_sprite.png", 1);
        sprite->SetDrawingMode(SpriteSceneNode::SpriteDrawingMode::Additive);
        sprite->SetLocalPosition({float(std::rand() % 800) / 100.0f - 4.0f, float(std::rand() % 600) / 100.0f - 3.0f, 0.0f});
        sprite->SetLocalScale(0.2f);
        backgroundLayers[0]->AddChild(sprite);
        entityMgr->CreateBackgroundWanderer(sprite);
    }
}

void BackgroundManager::Update(double dt)
{
    static double time = 0.0;
    time += dt;

    // need to take player position
    MathLib::vec2f playerPosition = GoingHome::GetGamePtr()->GetPlayerPtr()->GetPosition();
    for (int i = 0; i < 3; ++i)
    {
        const float k = (i + 2.0f) / 4.0f;
        backgroundLayers[i]->SetLocalPosition({playerPosition.x * k, playerPosition.y * k, 0.0f});
    }
}
