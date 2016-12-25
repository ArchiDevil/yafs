#pragma once

#include "../Entities/LiveEntity.h"
#include "ISpellEntity.h"

#include <cassert>

class LiveEntity;

class ISpellController
{
public:
    ISpellController(LiveEntity* caster)
        : caster(caster)
    {
        assert(caster);
    }

    virtual ~ISpellController() = default;

    virtual void SpellKeyDown() = 0;
    virtual void SpellKeyUp() = 0;
    virtual void Update(double dt) = 0;

protected:
    LiveEntity* caster = nullptr;

};
