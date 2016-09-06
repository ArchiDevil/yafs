#pragma once

#include "LiveEntity.h"

#include <MathLib/math.h>

class Enemy : public LiveEntity
{
    enum class EnemyState
    {
        Waiting,
        Moving
    };

public:
    Enemy(const MathLib::vec2f & position, float health, int expCount);
    virtual ~Enemy() = default;
    void Update(double dt) override;
    void MoveTo(const MathLib::vec2f & target);

private:
    MathLib::vec2f movePosition;
    EnemyState state = EnemyState::Waiting;

};
