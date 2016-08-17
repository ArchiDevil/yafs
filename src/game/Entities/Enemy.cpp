#include "Enemy.h"
#include "Projectile.h"

#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"enemy_sprite.png";
constexpr float MOVE_EPS = 0.05f;

Enemy::Enemy(const MathLib::vec2f & position, float health)
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
    , health(health)
{
    sprite->SetLocalScale(0.5f);
}

bool Enemy::handleEvent(const ProjectilePositionEvent & event)
{
    if (event.projectile->GetProducer() == this)
        return false;

    if (CalculateCollision(*event.projectile))
    {
        health -= event.projectile->GetDamage();
        event.projectile->Kill();

        if (health <= 0)
            Kill();
    }
    return true;
}

void Enemy::Update(double dt)
{
    if (state == EnemyState::Moving)
    {
        MathLib::vec2f direction = MathLib::normalize(movePosition - Entity::position);
        Entity::position += direction * dt; //* speed;
        UpdateGraphicsSpritePosition();

        MathLib::vec2f diff = movePosition - Entity::position;
        if (abs(diff.x) < MOVE_EPS && abs(diff.y) < MOVE_EPS)
        {
            state = EnemyState::Waiting;
        }
    }
}

void Enemy::MoveTo(const MathLib::vec2f & target)
{
    state = EnemyState::Moving;
    movePosition = target;
}
