#pragma once

#include <memory>
#include <Utilities/observer.h>
#include <Utilities/singleton.h>

class Projectile;

struct ProjectilePositionEvent
{
    ProjectilePositionEvent(const Projectile * projectile)
        : projectile(projectile)
    {
    }

    const Projectile * const projectile;
};

class EntityEventManager
    : public singleton <EntityEventManager>
    , public notifier <ProjectilePositionEvent>
{

};
