#pragma once

#include "EntityEventManager.h"

#include <GraphicsEngine/SceneGraph/SpriteSceneNode.h>
#include <MathLib/math.h>
#include <Utilities/observer.h>

#include <memory>

class Entity
    : public observer <ProjectilePositionEvent>
    , public observer <ExperiencePointPositionEvent>
{
public:
    Entity(const MathLib::vec2f & position,
           ShiftEngine::SpriteSceneNode * sprite);
    virtual ~Entity();

    virtual void Update(double dt) = 0;

    bool observer<ProjectilePositionEvent>::handleEvent(const ProjectilePositionEvent & event) override;
    bool observer<ExperiencePointPositionEvent>::handleEvent(const ExperiencePointPositionEvent & event) override;

    const MathLib::vec2f GetPosition() const;
    void SetPosition(MathLib::vec2f & pos);
    bool IsDead() const;
    void Kill();
    void UpdateGraphicsSpritePosition();

protected:
    bool CalculateCollision(const Entity & ent) const;

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
    scoped_subscriber<ExperiencePointPositionEvent> experiencePointSubscriber = { &EntityEventManager::GetInstance(), this };

};
