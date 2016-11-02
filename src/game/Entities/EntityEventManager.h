#pragma once

#include <memory>

#include <Utilities/observer.h>
#include <Utilities/singleton.h>

#include <MathLib/math.h>

class Projectile;
class ExperiencePoint;
class LiveEntity;

struct ProjectilePositionEvent final
{
    ProjectilePositionEvent(Projectile * projectile)
        : projectile(projectile)
    {
    }

    Projectile * projectile = nullptr;
};

struct ExperiencePointPositionEvent final
{
    ExperiencePointPositionEvent(ExperiencePoint * expPoint)
        : expPoint(expPoint)
    {
    }

    ExperiencePoint * expPoint = nullptr;
};

class EntityEventManager final
    : public singleton <EntityEventManager>
    , public notifier <ProjectilePositionEvent>
    , public notifier <ExperiencePointPositionEvent>
{
};
