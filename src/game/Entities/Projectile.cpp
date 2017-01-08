#include "Projectile.h"
#include "EntityEventManager.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"sprite.png";

Projectile::Projectile(const MathLib::vec2f & position, const MathLib::vec2f & speed, float damage, double lifetime, const LiveEntity * producer)
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
    , speed(speed)
    , producer(producer)
    , remainingTime(lifetime)
    , damage(damage)
{
    sprite->SetLocalScale(0.33f);
}

void Projectile::Update(double dt)
{
    remainingTime -= dt;
    if (remainingTime < 0.0)
    {
        Kill();
        return;
    }

    position += speed * dt;
    UpdateGraphicsSpritePosition();

    float overallIntensity = (float)remainingTime / 3.0f;
    sprite->SetMaskColor({overallIntensity, overallIntensity, overallIntensity, 1.0f});

    ((notifier<ProjectilePositionEvent>)EntityEventManager::GetInstance())
        .notifyAll(ProjectilePositionEvent(this));
}

const LiveEntity* Projectile::GetProducer() const
{
    return producer;
}

float Projectile::GetDamage() const
{
    return damage;
}

void Projectile::SetDamage(float val)
{
    damage = val;
}

MathLib::vec2f Projectile::GetSpeed() const
{
    return speed;
}
