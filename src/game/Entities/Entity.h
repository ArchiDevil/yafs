#pragma once

#include "EntityEventManager.h"

#include <GraphicsEngine/SceneGraph/SpriteSceneNode.h>
#include <MathLib/math.h>
#include <Utilities/observer.h>

#include <memory>

class Entity
{
public:
    Entity(const MathLib::vec2f & position,
           ShiftEngine::SpriteSceneNode * sprite);
    virtual ~Entity() = default;

    virtual void Update(double dt) = 0;
    // this should be removes as soon as possible and moved to some derivative
    // but this works for now, and will work before any architecture changes
    virtual void TakeDamage(float damageCount) { damageCount; }

    virtual MathLib::vec2f GetPosition() const;
    virtual void SetPosition(MathLib::vec2f position);

    bool IsDead() const;
    virtual void Kill();

protected:
    void UpdateGraphicsSpritePosition();
    bool CalculateCollision(const Entity & ent) const;

    struct sprites_deleter
    {
        void operator()(ShiftEngine::SpriteSceneNode* ref)
        {
            ref->KillSelf();
        }
    };

    bool isToDelete = false;
    std::unique_ptr<ShiftEngine::SpriteSceneNode, sprites_deleter> sprite = nullptr;

private:
    MathLib::vec2f position;

};
