#include "SpellEntities.h"

#include "../Game.h"

using namespace GoingHome;
using namespace MathLib;

ProjectileSpellEntity::ProjectileSpellEntity(float angleSpread, float speed, float damage, double lifetime)
    : angleSpread(angleSpread)
    , speed(speed)
    , damage(damage)
    , lifetime(lifetime)
{
}

void ProjectileSpellEntity::Cast(const LiveEntity * caster, const vec2f & direction)
{
    vec2f internalDirection = MathLib::normalize(direction);
    float angleFactor = (float)(rand() % (int)(angleSpread * 200) - (angleSpread * 100)) / 100.0f;
    internalDirection = vec2Transform(internalDirection, matrixRotationZ(angleFactor));
    GetGamePtr()->GetEntityMgr()->CreateProjectile(caster->GetPosition(), internalDirection * speed, damage, lifetime, caster);
}

//////////////////////////////////////////////////////////////////////////

MultiProjectileSpellEntity::MultiProjectileSpellEntity(float angleSpread, float speed, float damage, double lifetime, float spreadValue)
    : ProjectileSpellEntity(angleSpread, speed, damage, lifetime)
    , spreadValue(spreadValue)
{
}

void MultiProjectileSpellEntity::Cast(const LiveEntity * caster, const MathLib::vec2f & direction)
{
    for (float i = -spreadValue; i < spreadValue * 2; i += spreadValue)
    {
        ProjectileSpellEntity::Cast(caster, vec2Transform(direction, matrixRotationZ(i)));
    }
}

//////////////////////////////////////////////////////////////////////////

DetectorMinePeriodicCastSpellEntity::DetectorMinePeriodicCastSpellEntity(float explosionDamage)
    : explosionDamage(explosionDamage)
{
}

void DetectorMinePeriodicCastSpellEntity::Cast(const LiveEntity * caster)
{
    GetGamePtr()->GetEntityMgr()->CreateDetectorMine(caster, caster->GetPosition(), explosionDamage, 2.0f, 1.0f);
}

//////////////////////////////////////////////////////////////////////////

TimedMinePeriodicCastSpellEntity::TimedMinePeriodicCastSpellEntity(float explosionDamage, double time)
    : explosionDamage(explosionDamage)
    , time(time)
{
}

void TimedMinePeriodicCastSpellEntity::Cast(const LiveEntity * caster)
{
    GetGamePtr()->GetEntityMgr()->CreateTimedMine(caster, caster->GetPosition(), explosionDamage, 2.0f, 1.0f, time);
}
