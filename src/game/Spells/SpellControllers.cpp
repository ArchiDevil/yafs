#include "SpellControllers.h"

PeriodicCastSpellController::PeriodicCastSpellController(LiveEntity* caster,
                                                         const PeriodicCastInfo& castInfo,
                                                         std::unique_ptr<PeriodicCastSpellEntity> && spellEntity)
    : ISpellController(caster)
    , castInfo(castInfo)
    , spellEntity(std::move(spellEntity))
{
}

void PeriodicCastSpellController::SpellKeyDown()
{
    castStarted = true;
}

void PeriodicCastSpellController::SpellKeyUp()
{
    castStarted = false;
}

void PeriodicCastSpellController::Update(double dt)
{
    if (remainingCooldown >= 0.0)
        remainingCooldown -= dt;

    if (!castStarted || remainingCooldown >= 0.0)
        return;

    spellEntity->Cast(caster);

    remainingCooldown = castInfo.cooldown;
    // UNDONE: remove energy from caster
}

//////////////////////////////////////////////////////////////////////////

DirectedPeriodicCastSpellController::DirectedPeriodicCastSpellController(LiveEntity* caster,
                                                                         const PeriodicCastInfo& castInfo,
                                                                         std::unique_ptr<DirectedPeriodicCastSpellEntity> && spellEntity)
    : ISpellController(caster)
    , castInfo(castInfo)
    , spellEntity(std::move(spellEntity))
{
}

void DirectedPeriodicCastSpellController::SpellKeyDown()
{
    castStarted = true;
}

void DirectedPeriodicCastSpellController::SpellKeyUp()
{
    castStarted = false;
}

void DirectedPeriodicCastSpellController::Update(double dt)
{
    if (remainingCooldown >= 0.0)
        remainingCooldown -= dt;

    if (!castStarted || remainingCooldown >= 0.0)
        return;

    spellEntity->Cast(caster, caster->GetTargetDirection());

    remainingCooldown = castInfo.cooldown;
    // UNDONE: remove energy from caster
}
