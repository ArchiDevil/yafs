#include "Projectile.h"

#include "EntityEventManager.h"
#include "../Common.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"sprite.png";

using namespace ShiftEngine;

Projectile::Projectile(const MathLib::vec2f position,
                       float damage,
                       double lifetime,
                       const LiveEntity * producer,
                       const std::shared_ptr<Physics::Entity>& physicsEntity,
                       float size)
    : PhysicsEntity(position, GetSceneGraph()->AddSpriteNode(textureName, SL_Entities), physicsEntity)
    , producer(producer)
    , remainingTime(lifetime)
    , damage(damage)
{
    sprite->SetLocalScale(size);
}

void Projectile::Update(double dt)
{
    remainingTime -= dt;
    if (remainingTime < 0.0)
    {
        Kill();
        return;
    }

    PhysicsEntity::Update(dt);

    float overallIntensity = (float)remainingTime / 3.0f;
    sprite->SetMaskColor({ overallIntensity, overallIntensity, overallIntensity, 1.0f });
}

const LiveEntity* Projectile::GetProducer() const
{
    return producer;
}

MathLib::vec2f Projectile::GetPosition() const
{
    return IPhysicsEntityHolder::physicsEntity->GetPosition();
}

MathLib::vec2f Projectile::GetSpeed() const
{
    return IPhysicsEntityHolder::physicsEntity->GetVelocity();
}

void Projectile::OnCollision(Physics::IPhysicsEntityHolder* other)
{
    Projectile* projectileCollider = dynamic_cast<Projectile*>(other);
    if (projectileCollider)
        return;

    Entity *collider = dynamic_cast<Entity*>(other);
    if (collider && producer != collider)
    {
        collider->TakeDamage(damage);
        Entity::Kill();
    }
}
