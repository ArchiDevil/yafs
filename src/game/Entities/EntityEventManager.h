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

struct LiveEntityPositionEvent final
{
    LiveEntityPositionEvent(LiveEntity * producer)
        : producer(producer)
    {
    }

    LiveEntity * producer = nullptr;
};

struct ExplosionEvent final
{
    ExplosionEvent(const MathLib::vec2f & epicenter, float damage, float radius)
        : epicenter(epicenter)
        , damage(damage)
        , radius(radius)
    {
    }

    MathLib::vec2f epicenter;
    float damage = 0.0f;
    float radius = 0.0f;
};

// TODO: we need to rework this, because amount of events will grow fast
class EntityEventManager final
    : public singleton<EntityEventManager>
    , public notifier<ProjectilePositionEvent>
    , public notifier<ExperiencePointPositionEvent>
    , public notifier<LiveEntityPositionEvent>
    , public notifier<ExplosionEvent>
{
};
