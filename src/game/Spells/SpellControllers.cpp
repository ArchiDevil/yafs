#include "SpellControllers.h"

PeriodicCastSpellController::PeriodicCastSpellController(LiveEntity* caster,
                                                         const PeriodicCastInfo& castInfo,
                                                         std::unique_ptr<PeriodicCastSpellEntity> && spellEntity)
    : ISpellController(caster)
    , castInfo(castInfo)
    , spellEntity(std::move(spellEntity))
{
}

PeriodicCastSpellController::PeriodicCastSpellController(LiveEntity* caster,
                                                         const PeriodicCastInfo& castInfo)
    : ISpellController(caster)
    , castInfo(castInfo)
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

    OnCast();

    remainingCooldown = castInfo.cooldown;
    // UNDONE: remove energy from caster
}

void PeriodicCastSpellController::OnCast()
{
    if (spellEntity)
        spellEntity->Cast(caster);
}

//////////////////////////////////////////////////////////////////////////

DirectedPeriodicCastSpellController::DirectedPeriodicCastSpellController(LiveEntity* caster,
                                                                         const PeriodicCastInfo& castInfo,
                                                                         std::unique_ptr<DirectedPeriodicCastSpellEntity> && spellEntity)
    : PeriodicCastSpellController(caster, castInfo)
    , spellEntity(std::move(spellEntity))
{
}

void DirectedPeriodicCastSpellController::OnCast()
{
    if (spellEntity)
        spellEntity->Cast(caster, caster->GetTargetDirection());
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
    if (spellEntity)
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
    if (spellEntity)
        spellEntity->StopCast(ISpellController::caster);
}
