#pragma once

#include <memory>
#include <Utilities/observer.h>
#include <Utilities/singleton.h>

class Projectile;

struct ProjectilePositionEvent
{
    ProjectilePositionEvent(Projectile * projectile)
        : projectile(projectile)
    {
    }

    Projectile * projectile;
};

class EntityEventManager
    : public singleton <EntityEventManager>
    , public notifier <ProjectilePositionEvent>
{

};
