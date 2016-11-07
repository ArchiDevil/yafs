#pragma once

#include "ISpellEntity.h"
#include "../Entities/Buffs.h"

//////////////////////////////////////////////////////////////////////////

class PeriodicCastSpellEntity : public ISpellEntity
{
public:
    virtual void Cast(const LiveEntity * caster) = 0;
};

//////////////////////////////////////////////////////////////////////////

class DirectedPeriodicCastSpellEntity : public ISpellEntity
{
public:
    virtual void Cast(const LiveEntity * caster, const MathLib::vec2f & direction) = 0;
};

class DetectorMinePeriodicCastSpellEntity final : public PeriodicCastSpellEntity
{
public:
    DetectorMinePeriodicCastSpellEntity(float explosionDamage);
    void Cast(const LiveEntity * caster) override;

protected:
    float explosionDamage = 0.0f;
};

class TimedMinePeriodicCastSpellEntity final : public PeriodicCastSpellEntity
{
public:
    TimedMinePeriodicCastSpellEntity(float explosionDamage, double time);
    void Cast(const LiveEntity * caster) override;

protected:
    float explosionDamage = 0.0f;
    double time = 0.0;
};

class ProjectileSpellEntity : public DirectedPeriodicCastSpellEntity
{
public:
    ProjectileSpellEntity(float angleSpread, float speed, float damage, double lifetime);
    void Cast(const LiveEntity * caster, const MathLib::vec2f & direction) override;

protected:
    float angleSpread = 15.0f;
    float speed = 2.0f;
    float damage = 1.0f;
    double lifetime = 3.0;
};

class MultiProjectileSpellEntity final: public ProjectileSpellEntity
{
public:
    MultiProjectileSpellEntity(float angleSpread, float speed, float damage, double lifetime, float spreadValue);
    void Cast(const LiveEntity * caster, const MathLib::vec2f & direction) override;

protected:
    float spreadValue = 0.15f;
};

//////////////////////////////////////////////////////////////////////////

class ChannelledCastSpellEntity : public ISpellEntity
{
public:
    virtual void StartCast(LiveEntity * caster) = 0;
    virtual void StopCast(LiveEntity * caster) = 0;
};

class ChannelledShieldSpellEntity final : public ChannelledCastSpellEntity
{
public:
    void StartCast(LiveEntity * caster) override;
    void StopCast(LiveEntity * caster) override;

private:
    std::shared_ptr<IBuff> shieldBuff = std::make_shared<ShieldBuff>();
};

//////////////////////////////////////////////////////////////////////////

class ChannelledDirectedCastSpellEntity : public ISpellEntity
{
public:
    virtual void StartCast(LiveEntity * caster, const MathLib::vec2f & direction) = 0;
    virtual void StopCast(LiveEntity * caster, const MathLib::vec2f & direction) = 0;
};

//////////////////////////////////////////////////////////////////////////
