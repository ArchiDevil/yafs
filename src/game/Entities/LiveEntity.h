#pragma once

#include "Entity.h"

class LiveEntity : public Entity
{
public:
    LiveEntity(const MathLib::vec2f & position, float health, const std::wstring & textureName, int expCount);
    virtual ~LiveEntity() = default;

    bool observer<ProjectilePositionEvent>::handleEvent(const ProjectilePositionEvent & event) override;
    bool observer<ExperiencePointPositionEvent>::handleEvent(const ExperiencePointPositionEvent & event) override;
    void Shoot(const MathLib::vec2f & targetPosition);
    int GetExperienceCount();

protected:
    float health = 1.0f;
    int experienceCount = 0;

};
