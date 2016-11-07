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

//////////////////////////////////////////////////////////////////////////

ChanneledCastSpellController::ChanneledCastSpellController(LiveEntity* caster,
                                                           const ChanneledCastInfo& castInfo,
                                                           std::unique_ptr<ChannelledCastSpellEntity> && spellEntity)
    : ISpellController(caster)
    , castInfo(castInfo)
    , spellEntity(std::move(spellEntity))
{
}

void ChanneledCastSpellController::SpellKeyDown()
{
    if (remainingCooldown > 0 || castStarted)
        return;

    castStarted = true;
    spellEntity->StartCast(ISpellController::caster);
}

void ChanneledCastSpellController::SpellKeyUp()
{
    if (!castStarted)
        return;

    StopCast();
}

void ChanneledCastSpellController::Update(double dt)
{
    if (remainingCooldown >= 0.0)
        remainingCooldown -= dt;

    if (!castStarted)
        return;

    castTime += dt;
    if (castTime >= castInfo.maximumUsageTime)
    {
        StopCast();
    }

    // UNDONE: remove energy from caster
}

void ChanneledCastSpellController::StopCast()
{
    remainingCooldown = castInfo.cooldown;
    castTime = 0.0;
    castStarted = false;
    spellEntity->StopCast(ISpellController::caster);
}
