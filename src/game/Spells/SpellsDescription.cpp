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
                                                                 std::make_unique<ProjectileSpellEntity>(spread, 1.0f, damage, 2.0));
}
