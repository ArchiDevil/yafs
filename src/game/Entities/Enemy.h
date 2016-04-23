#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Enemy : public Entity
{
public:
    Enemy(MathLib::vec2f & position);
    virtual ~Enemy() { }
    std::string GetName() override;
    bool handleEvent(const EntityEvent & event) override;

};