#include "Enemy.h"
#include "Projectile.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"enemy";

Enemy::Enemy(const MathLib::vec2f & position)
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
{
}

bool Enemy::handleEvent(const ProjectilePositionEvent & event)
{
    if (CalculateCollision(*event.projectile))
    {
        Die();
        event.projectile->Die();
    }
    return true;
}

void Enemy::Update(double dt)
{
    dt;
    SetSpritePosition();
}
