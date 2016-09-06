#pragma once

#include <memory>
#include <Utilities/observer.h>
#include <Utilities/singleton.h>

class Projectile;
class ExperiencePoint;

struct ProjectilePositionEvent
{
    ProjectilePositionEvent(Projectile * projectile)
        : projectile(projectile)
    {
    }

    Projectile * projectile;
};

struct ExperiencePointPositionEvent
{
    ExperiencePointPositionEvent(ExperiencePoint * expPoint)
        : expPoint(expPoint)
    {
    }

    ExperiencePoint * expPoint;
};

class EntityEventManager
    : public singleton <EntityEventManager>
    , public notifier <ProjectilePositionEvent>
    , public notifier <ExperiencePointPositionEvent>
{

};
