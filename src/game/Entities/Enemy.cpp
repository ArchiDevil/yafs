#include "Enemy.h"
#include "Projectile.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring enemyTextureName = L"enemy_sprite.png";
constexpr float MOVE_EPS = 0.05f;

Enemy::Enemy(MathLib::vec2f position,
             float health,
             int expCount,
             const std::shared_ptr<Physics::Entity>& physicsEntity,
             std::unique_ptr<AIBase> && ai)
    : LiveEntity(position, health, enemyTextureName, expCount, physicsEntity)
    , ai(std::move(ai))
{
    sprite->SetLocalScale(0.5f);
}

void Enemy::Update(double dt)
{
    if (ai != nullptr)
    {
        ai->Update(dt, this);
    }
    LiveEntity::Update(dt);
}

void Enemy::MoveTo(const MathLib::vec2f & target)
{
    movePosition = target;
}
