#include "Projectile.h"
#include "EntityEventManager.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"sprite.png";

Projectile::Projectile(const MathLib::vec2f & position, const MathLib::vec2f & speed) 
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
    , speed(speed)
{
}

void Projectile::Update(double dt)
{
    lifetime += dt;

    position += speed * dt;
    SetSpritePosition();

    EntityEventManager::GetInstance().notifyAll(ProjectilePositionEvent(this));
    
    if (lifetime > 3)
    {
        Kill();
    }
}
