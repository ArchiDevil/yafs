#include "Projectile.h"
#include "EntityEventManager.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"projectile";

Projectile::Projectile(MathLib::vec2f & position, MathLib::vec2f & speed)
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
    , speed(speed)
{
}

void Projectile::Update(double dt)
{
    position += speed * dt;

    EntityEventManager::GetInstance().notifyAll(ProjectilePositionEvent(this));
}
