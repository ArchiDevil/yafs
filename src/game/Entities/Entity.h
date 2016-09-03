#pragma once

#include "EntityEventManager.h"

#include <GraphicsEngine/SceneGraph/SpriteSceneNode.h>
#include <MathLib/math.h>
#include <Utilities/observer.h>

#include <memory>

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

    bool handleEvent(const ProjectilePositionEvent & event) override;

    const MathLib::vec2f GetPosition() const;
    bool IsDead() const;
    void Kill();

protected:
    bool CalculateCollision(const Entity & ent) const;
    void UpdateGraphicsSpritePosition();

    struct sprites_deleter
    {
        void operator()(ShiftEngine::SpriteSceneNode* ref)
        {
            ref->KillSelf();
        }
    };

    MathLib::vec2f position;
    bool isToDelete = false;
    std::unique_ptr<ShiftEngine::SpriteSceneNode, sprites_deleter> sprite = nullptr;
    scoped_subscriber<ProjectilePositionEvent> projectileSubscriber = {&EntityEventManager::GetInstance(), this};

};
