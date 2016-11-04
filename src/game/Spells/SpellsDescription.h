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

class MultiProjectileSpellDescription final : public ISpellDescription
{
public:
    MultiProjectileSpellDescription(const std::string & name, float cooldown, float damage, float spread, unsigned count, float per_bullet_spread, float energy);
    std::unique_ptr<ISpellController> CreateSpellController(LiveEntity* caster) const override;

private:
    const float damage = 0.0f;
    const float spread = 0.0f;
    const unsigned count = 0;
    const float per_bullet_spread = 0.0f;
    const float energy = 0.0f;
};

class DetectorMineSpellDescription final : public ISpellDescription
{
public:
    DetectorMineSpellDescription(const std::string & name, float cooldown, float damage, float energy);
    std::unique_ptr<ISpellController> CreateSpellController(LiveEntity* caster) const override;

private:
    const float damage = 0.0f;
    const float energy = 0.0f;
};

class TimedMineSpellDescription final : public ISpellDescription
{
public:
    TimedMineSpellDescription(const std::string & name, float cooldown, float damage, float energy, double time);
    std::unique_ptr<ISpellController> CreateSpellController(LiveEntity* caster) const override;

private:
    const float damage = 0.0f;
    const float energy = 0.0f;
    const double time = 0.0;
};
