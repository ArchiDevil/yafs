#include "../Game.h"
#include "Player.h"
#include "Projectile.h"
#include "EntityFactory.h"

#include <cmath>
#include <GraphicsEngine/ShiftEngine.h>

const std::wstring playerTextureName = L"player_sprite.png";

Player::Player(const MathLib::vec2f & position, float health)
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(playerTextureName))
    , health(health)
{
}

bool Player::handleEvent(const ProjectilePositionEvent & event)
{
    if (event.projectile->GetProducer() == this)
        return false;

    // code is the same with enemy... that is shitty
    if (CalculateCollision(*event.projectile))
    {
        health -= event.projectile->GetDamage();
        event.projectile->Kill();

        if (health <= 0)
            Kill();
    }

    return true;
}

void Player::Shoot(const MathLib::vec2f & targetPosition)
{
    auto vec = MathLib::normalize(targetPosition - position);
    GoingHome::GetGamePtr()->GetEntityMgr()->CreateProjectile(position, vec * 2.0f, 1.0f, 3.0, this);
}

void Player::SetMoveVelocity(const MathLib::vec2f & velocity)
{
    moveVelocity = velocity;
}

void Player::Update(double dt)
{
    position += moveVelocity * dt;
    UpdateGraphicsSpritePosition();
}
