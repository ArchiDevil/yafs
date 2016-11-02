#pragma once

#include <vector>
#include <memory>

#include <MathLib/math.h>

#include "../Entities/LiveEntity.h"
#include "ISpellController.h"

class ISpellDescription
{
public:
    enum class SpellType
    {
        Projectile,
        Multiprojectile,
        Mine,
        Shield
    };

    SpellType GetSpellType() const
    {
        return type;
    }

    const std::string & GetSpellName() const
    {
        return name;
    }

    ISpellDescription(SpellType type, double cooldown, const std::string & name)
        : type(type)
        , cooldown(cooldown)
        , name(name)
    {
    }

    virtual std::unique_ptr<ISpellController> CreateSpellController(LiveEntity* caster) const = 0;

protected:
    const std::string name;
    const SpellType type;
    const double cooldown;

};
