#pragma once

#include "Entity.h"

class LiveEntity : public Entity
{
public:
    LiveEntity(const MathLib::vec2f & position, float health, const std::wstring & textureName);
    virtual ~LiveEntity() = default;

    bool handleEvent(const ProjectilePositionEvent & event) override;
    void Shoot(const MathLib::vec2f & targetPosition);

private:
    float health = 1.0f;

};
