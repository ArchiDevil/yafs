#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Enemy : public Entity
{
    enum class EnemyState
    {
        Waiting,
        Moving
    };

public:
    Enemy(const MathLib::vec2f & position);
    virtual ~Enemy() = default;
    bool handleEvent(const ProjectilePositionEvent & event) override;
    void Update(double dt) override;
    void MoveTo(const MathLib::vec2f & target);

private:
    MathLib::vec2f movePosition;
    EnemyState state = EnemyState::Waiting;

};
