#include "SpellsDescription.h"

#include "SpellEntities.h"
#include "SpellControllers.h"

ProjectileSpellDescription::ProjectileSpellDescription(const std::string & name,
                                                       float cooldown,
                                                       float damage,
                                                       float spread,
                                                       float energy)
    : ISpellDescription(ISpellDescription::SpellType::Projectile, cooldown, name)
    , damage(damage)
    , spread(spread)
    , energy(energy)
{
}

std::unique_ptr<ISpellController> ProjectileSpellDescription::CreateSpellController(LiveEntity* caster) const
{
    return std::make_unique<DirectedPeriodicCastSpellController>(caster,
                                                                 PeriodicCastInfo {ISpellDescription::cooldown, energy},
                                                                 std::make_unique<ProjectileSpellEntity>(spread, 4.0f, damage, 2.0));
}

//////////////////////////////////////////////////////////////////////////

MultiProjectileSpellDescription::MultiProjectileSpellDescription(const std::string & name,
                                                                 float cooldown,
                                                                 float damage,
                                                                 float spread,
                                                                 unsigned count,
                                                                 float per_bullet_spread,
                                                                 float energy)
    : ISpellDescription(ISpellDescription::SpellType::Multiprojectile, cooldown, name)
    , damage(damage)
    , spread(spread)
    , count(count)
    , per_bullet_spread(per_bullet_spread)
    , energy(energy)
{
}

std::unique_ptr<ISpellController> MultiProjectileSpellDescription::CreateSpellController(LiveEntity* caster) const
{
    return std::make_unique<DirectedPeriodicCastSpellController>(caster,
                                                                 PeriodicCastInfo {ISpellDescription::cooldown, energy},
                                                                 std::make_unique<MultiProjectileSpellEntity>(spread, 4.0f, damage, 2.0, per_bullet_spread));
}

//////////////////////////////////////////////////////////////////////////

DetectorMineSpellDescription::DetectorMineSpellDescription(const std::string & name, float cooldown, float damage, float energy)
    : ISpellDescription(ISpellDescription::SpellType::Mine, cooldown, name)
    , damage(damage)
    , energy(energy)
{
}

std::unique_ptr<ISpellController> DetectorMineSpellDescription::CreateSpellController(LiveEntity* caster) const
{
    return std::make_unique<PeriodicCastSpellController>(caster,
                                                         PeriodicCastInfo {ISpellDescription::cooldown, energy},
                                                         std::make_unique<DetectorMinePeriodicCastSpellEntity>(damage));
}

//////////////////////////////////////////////////////////////////////////

TimedMineSpellDescription::TimedMineSpellDescription(const std::string & name, float cooldown, float damage, float energy, double time)
    : ISpellDescription(ISpellDescription::SpellType::Mine, cooldown, name)
    , damage(damage)
    , energy(energy)
    , time(time)
{
}

std::unique_ptr<ISpellController> TimedMineSpellDescription::CreateSpellController(LiveEntity* caster) const
{
    return std::make_unique<PeriodicCastSpellController>(caster,
                                                         PeriodicCastInfo {ISpellDescription::cooldown, energy},
                                                         std::make_unique<TimedMinePeriodicCastSpellEntity>(damage, time));
}

//////////////////////////////////////////////////////////////////////////

ShieldSpellDescription::ShieldSpellDescription(const std::string & name,
                                               float cooldown,
                                               float usage_time,
                                               float energy_per_second)
    : ISpellDescription(ISpellDescription::SpellType::Shield, cooldown, name)
    , usage_time(usage_time)
    , energy_per_second(energy_per_second)
{
}

std::unique_ptr<ISpellController> ShieldSpellDescription::CreateSpellController(LiveEntity* caster) const
{
    // damage reflection amount?
    return std::make_unique<ChanneledCastSpellController>(caster,
                                                          ChanneledCastInfo {ISpellDescription::cooldown, energy_per_second, usage_time},
                                                          std::make_unique<ChannelledShieldSpellEntity>());
}
