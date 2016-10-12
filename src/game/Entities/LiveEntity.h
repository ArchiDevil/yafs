#pragma once

#include "Entity.h"

class LiveEntity : public Entity
{
public:
    enum Fraction
    {
        FractionPlayer,
        FractionEnemy
    };

    LiveEntity(const MathLib::vec2f & position, float health, const std::wstring & textureName, int expCount, Fraction fract = FractionEnemy);
    virtual ~LiveEntity() = default;

    bool observer<ProjectilePositionEvent>::handleEvent(const ProjectilePositionEvent & event) override;
    bool observer<ExperiencePointPositionEvent>::handleEvent(const ExperiencePointPositionEvent & event) override;
    void Shoot(const MathLib::vec2f & targetPosition);
    const int GetExperienceCount();
    const float GetMaxHealth();
    const float GetHealth();
    const Fraction GetFraction();

protected:
    float maxHealth = 1.0f;
    float health = 1.0f;
    int experienceCount = 0;
    Fraction fraction = FractionEnemy;
};
