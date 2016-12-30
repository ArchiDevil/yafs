#pragma once

#include "LiveEntity.h"

#include <MathLib/math.h>

class Enemy final
    : public LiveEntity
{
    enum class EnemyState
    {
        Waiting,
        Moving
    };

public:
    Enemy(MathLib::vec2f position,
          float health,
          int expCount,
          const std::shared_ptr<Physics::Entity>& physicsEntity);

    virtual ~Enemy() = default;
    void Update(double dt) override;
    void MoveTo(const MathLib::vec2f & target);

private:
    MathLib::vec2f movePosition;
    EnemyState state = EnemyState::Waiting;

};
