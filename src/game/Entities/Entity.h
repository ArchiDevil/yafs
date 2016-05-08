#pragma once

#include "EntityEventManager.h"

#include <MathLib/math.h>
#include <string>
#include <Utilities/observer.h>

class Entity
    : public observer <ProjectilePositionEvent>
{
public:
    Entity(MathLib::vec2f & position);
    virtual ~Entity();

    virtual void Update(double dt) = 0;
    virtual void Show() { }
    virtual void Hide() { }
    virtual void Move(double x, double y) { }

    bool handleEvent(const ProjectilePositionEvent & event) override { return true; };

    const MathLib::vec2f GetPosition() const { return position; }
    bool IsToDelete() { return isToDelete; }
protected:
    bool CalculateCollision(const Entity & ent);

    MathLib::vec2f position;
    bool isToDelete = false;
};
