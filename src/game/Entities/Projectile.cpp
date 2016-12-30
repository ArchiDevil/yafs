#include "Projectile.h"
#include "EntityEventManager.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"sprite.png";

Projectile::Projectile(const MathLib::vec2f position,
                       float damage,
                       double lifetime,
                       const LiveEntity * producer,
                       const std::shared_ptr<Physics::Entity>& physicsEntity)
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
    , IPhysicsEntityHolder(physicsEntity)
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

    Entity::SetPosition(IPhysicsEntityHolder::physicsEntity->GetPosition());

    float overallIntensity = (float)remainingTime / 3.0f;
    sprite->SetMaskColor({ overallIntensity, overallIntensity, overallIntensity, 1.0f });
}

const LiveEntity* Projectile::GetProducer() const
{
    return producer;
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
