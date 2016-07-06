#pragma once

#include "EntityEventManager.h"

#include <GraphicsEngine/SceneGraph/SpriteSceneNode.h>
#include <MathLib/math.h>
#include <memory>
#include <string>
#include <Utilities/observer.h>

class Entity
    : public observer <ProjectilePositionEvent>
{
public:
    Entity(const MathLib::vec2f & position,
           ShiftEngine::SpriteSceneNode * sprite);
    virtual ~Entity();

    virtual void Update(double dt) = 0;
    virtual void Show();
    virtual void Hide();
    virtual void Move(double x, double y);

    bool handleEvent(const ProjectilePositionEvent & event) override;;

    const MathLib::vec2f GetPosition() const;
    bool IsDead();
    void Die();
protected:
    bool CalculateCollision(const Entity & ent) const;
    void SetSpritePosition();

    struct sprites_deleter
    {
        void operator()(ShiftEngine::SpriteSceneNode* ref)
        {
            ref->KillSelf();
        }
    };

    MathLib::vec2f position;
    bool isToDelete = false;
    std::unique_ptr<ShiftEngine::SpriteSceneNode, sprites_deleter> sprite;
    scoped_subscriber<ProjectilePositionEvent> projectileSubscriber = {&EntityEventManager::GetInstance(), this};
};
