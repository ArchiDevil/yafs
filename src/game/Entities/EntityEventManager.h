#pragma once

#include "Utilities/observer.h"
#include "Utilities/singleton.h"

enum class EntityEventType
{
    ProjectilePosition
};

struct EntityEvent
{
    EntityEvent(EntityEventType type, void * data)
        : type(type)
        , data(data)
    {
    }

    EntityEventType type;
    void * data;
};

class EntityEventManager
    : public singleton < EntityEventManager >
    , public notifier <EntityEvent>
{

};
