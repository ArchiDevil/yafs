#include "Enemy.h"
#include "Projectile.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring enemyTextureName = L"enemy_sprite.png";
constexpr float MOVE_EPS = 0.05f;

Enemy::Enemy(const MathLib::vec2f & position, float health, int expCount, const std::shared_ptr<AIBase> & ai)
    : LiveEntity(position, health, enemyTextureName, expCount)
    , ai(ai)
{
    sprite->SetLocalScale(0.5f);
}

void Enemy::Update(double dt)
{
    if (ai != nullptr)
    {
        ai->Update(dt, this);
    }
    /*if (state == EnemyState::Moving)
    {
        MathLib::vec2f direction = MathLib::normalize(movePosition - Entity::position);
        Entity::position += direction * dt; //* speed;
        UpdateGraphicsSpritePosition();

        MathLib::vec2f diff = movePosition - Entity::position;
        if (abs(diff.x) < MOVE_EPS && abs(diff.y) < MOVE_EPS)
        {
            state = EnemyState::Standing;
        }
    }*/
    LiveEntity::Update(dt);
}

void Enemy::MoveTo(const MathLib::vec2f & target)
{
    state = EnemyState::Moving;
    movePosition = target;
}
