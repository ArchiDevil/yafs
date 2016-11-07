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

//////////////////////////////////////////////////////////////////////////

struct ChanneledCastInfo
{
    ChanneledCastInfo(double cooldown, float energyPerSecondCost, double maximumUsageTime)
        : cooldown(cooldown)
        , energyPerSecondCost(energyPerSecondCost)
        , maximumUsageTime(maximumUsageTime)
    {
    }

    // time to refresh spell after single usage
    double cooldown = 0.0;

    // amount of energy required to cast this spell every second
    float energyPerSecondCost = 0.0f;

    // maximum time that user can cast this spell
    double maximumUsageTime = 0.0;
};

class ChanneledCastSpellController final : public ISpellController
{
public:
    ChanneledCastSpellController(LiveEntity* caster,
                                 const ChanneledCastInfo& castInfo,
                                 std::unique_ptr<ChannelledCastSpellEntity> && spellEntity);

    void SpellKeyDown() override;
    void SpellKeyUp() override;
    void Update(double dt) override;

private:
    void StopCast();

    bool castStarted = false;
    ChanneledCastInfo castInfo;
    std::unique_ptr<ChannelledCastSpellEntity> spellEntity = nullptr;
    double remainingCooldown = 0.0;
    double castTime = 0.0;

};

class ChanneledDirectedCastSpellController final : public ISpellController
{
public:
    ChanneledDirectedCastSpellController(LiveEntity* caster,
                                         const ChanneledCastInfo& castInfo,
                                         std::unique_ptr<ChannelledCastSpellEntity> && spellEntity);

    void SpellKeyDown() override;
    void SpellKeyUp() override;
    void Update(double dt) override;

private:
    bool castStarted = false;
    ChanneledCastInfo castInfo;
    std::unique_ptr<ChannelledCastSpellEntity> spellEntity = nullptr;
    double remainingCooldown = 0.0;
    double castTime = 0.0;

};
