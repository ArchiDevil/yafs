#include "Projectile.h"
#include "EntityEventManager.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"projectile";

Projectile::Projectile(const MathLib::vec2f & position, const MathLib::vec2f & speed) 
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
    , speed(speed)
{
}

void Projectile::Update(double dt)
{
    position += speed * dt;

    EntityEventManager::GetInstance().notifyAll(ProjectilePositionEvent(this));
}
