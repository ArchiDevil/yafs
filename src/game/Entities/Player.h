#pragma once

#include "LiveEntity.h"

#include <MathLib/math.h>

class Player final
    : public LiveEntity
{
public:
    Player(MathLib::vec2f position,
           float health,
           const std::shared_ptr<Physics::Entity>& physicsEntity);

    virtual ~Player() = default;

    void Update(double dt) override;
    void SetMoveVector(const MathLib::vec2f &vector);

private:
    MathLib::vec2f moveVector = {};

};
