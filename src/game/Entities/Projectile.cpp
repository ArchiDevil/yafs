#include "Projectile.h"
#include "EntityEventManager.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"sprite.png";

Projectile::Projectile(const MathLib::vec2f & position, const MathLib::vec2f & speed, Entity * producer)
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
    , speed(speed)
    , producer(producer)
{
}

void Projectile::Update(double dt)
{
    lifetime += dt;

    position += speed * dt;
    UpdateGraphicsSpritePosition();

    EntityEventManager::GetInstance().notifyAll(ProjectilePositionEvent(this));

    float overallIntensity = 1.0f - (float)lifetime / 3.0;
    sprite->SetMaskColor({overallIntensity, overallIntensity, overallIntensity, 1.0f});

    if (lifetime > 3.0)
    {
        Kill();
    }
}

Entity* Projectile::GetProducer() const
{
    return producer;
}
