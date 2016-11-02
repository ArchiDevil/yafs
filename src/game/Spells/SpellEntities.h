#pragma once

#include "ISpellEntity.h"

class DirectedPeriodicCastSpellEntity : public ISpellEntity
{
public:
    virtual void Cast(const LiveEntity * caster, const MathLib::vec2f & direction) = 0;
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
