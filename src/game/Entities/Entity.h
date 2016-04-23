#pragma once

#include "EntityEventManager.h"
#include "Utilities/observer.h"

#include <MathLib/math.h>
#include <string>

class Entity
    : public observer <EntityEvent>
{
public:
    Entity(MathLib::vec2f & position);
    virtual ~Entity();

    virtual std::string GetName() = 0;
    virtual void Update(double deltaTime) { }
    virtual void Show() { }
    virtual void Hide() { }
    virtual void Move(double x, double y) { }

    bool handleEvent(const EntityEvent & event) override { return true; };

    MathLib::vec2f GetPosition() { return position; }
    bool IsToDelete() { return isToDelete; }
protected:
    bool CalculateCollision(Entity & ent);

    MathLib::vec2f position;
    bool isToDelete = false;
};
