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

};
