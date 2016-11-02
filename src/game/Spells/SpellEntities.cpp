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
