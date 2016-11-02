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

class ProjectileSpellDescription final : public ISpellDescription
{
public:
    ProjectileSpellDescription(const std::string & name, float cooldown, float damage, float spread, float energy);
    std::unique_ptr<ISpellController> CreateSpellController(LiveEntity* caster) const override;

private:
    const float damage = 0.0f;
    const float spread = 0.0f;
    const float energy = 0.0f;
};
