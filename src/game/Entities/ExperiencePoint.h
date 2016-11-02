#pragma once

#include "Entity.h"

class ExperiencePoint final
    : public Entity
{

public:
    ExperiencePoint(const MathLib::vec2f & position, int expCount);
    virtual ~ExperiencePoint() = default;
    void Update(double dt) override;
    int GetExperienceCount() const;

private:
    int expCount;
    double remainingTime = 10.0;
};
