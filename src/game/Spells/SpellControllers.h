#pragma once

#include "ISpellController.h"
#include "SpellEntities.h"

struct PeriodicCastInfo
{
    PeriodicCastInfo(double cooldown, float energyCost)
        : cooldown(cooldown)
        , energyCost(energyCost)
    {
    }

    // time to refresh spell after single usage
    double cooldown = 0.0;

    // cost of spell for a single use
    float energyCost = 0.0f;
};

class PeriodicCastSpellController final : public ISpellController
{
public:
    PeriodicCastSpellController(LiveEntity* caster,
                                const PeriodicCastInfo& castInfo,
                                std::unique_ptr<PeriodicCastSpellEntity> && spellEntity);

    void SpellKeyDown() override;
    void SpellKeyUp() override;
    void Update(double dt) override;

private:
    bool castStarted = false;
    PeriodicCastInfo castInfo;
    std::unique_ptr<PeriodicCastSpellEntity> spellEntity = nullptr;
    double remainingCooldown = 0.0;

};

class DirectedPeriodicCastSpellController final : public ISpellController
{
public:
    DirectedPeriodicCastSpellController(LiveEntity* caster,
                                        const PeriodicCastInfo& castInfo,
                                        std::unique_ptr<DirectedPeriodicCastSpellEntity> && spellEntity);

    void SpellKeyDown() override;
    void SpellKeyUp() override;
    void Update(double dt) override;

private:
    bool castStarted = false;
    PeriodicCastInfo castInfo;
    std::unique_ptr<DirectedPeriodicCastSpellEntity> spellEntity = nullptr;
    double remainingCooldown = 0.0;

};
