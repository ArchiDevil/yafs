#include "Enemy.h"
#include "Projectile.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring enemyTextureName = L"enemy_sprite.png";
constexpr float MOVE_EPS = 0.05f;

Enemy::Enemy(MathLib::vec2f position,
             float health,
             int expCount,
             const std::shared_ptr<Physics::Entity>& physicsEntity)
    : LiveEntity(position, health, enemyTextureName, expCount, physicsEntity)
{
    sprite->SetLocalScale(0.5f);
}

void Enemy::Update(double dt)
{
    if (state == EnemyState::Moving)
    {
        // position adjusting
        MathLib::vec2f direction = MathLib::normalize(movePosition - Entity::GetPosition());
        IPhysicsEntityHolder::physicsEntity->SetVelocity(direction);

        // state switching
        MathLib::vec2f diff = movePosition - Entity::GetPosition();
        if (abs(diff.x) < MOVE_EPS && abs(diff.y) < MOVE_EPS)
        {
            state = EnemyState::Waiting;
            IPhysicsEntityHolder::physicsEntity->SetVelocity({});
        }
    }

    LiveEntity::Update(dt);
}

void Enemy::MoveTo(const MathLib::vec2f & target)
{
    state = EnemyState::Moving;
    movePosition = target;
}
