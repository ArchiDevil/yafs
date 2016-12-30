#pragma once

#include "LiveEntity.h"
#include "../AI/AIBase.h"

#include <MathLib/math.h>

#include <memory>

class Enemy final
    : public LiveEntity
{
public:
    enum class EnemyType
    {
        SmallSpirit,
    };

    Enemy(const MathLib::vec2f & position, float health, int expCount, std::unique_ptr<AIBase> && ai);
    virtual ~Enemy() = default;

    void Update(double dt) override;
    void MoveTo(const MathLib::vec2f & target);

private:
    MathLib::vec2f movePosition;
    std::unique_ptr<AIBase> ai;

};
