#include "../Game.h"
#include "Player.h"
#include "Projectile.h"
#include "EntityFactory.h"

#include <cmath>
#include <GraphicsEngine/ShiftEngine.h>

using namespace MathLib;
using namespace GoingHome;
using namespace ShiftEngine;

const std::wstring playerTextureName = L"player_sprite.png";

Player::Player(const vec2f & position, float health)
    : Entity(position, GetSceneGraph()->AddSpriteNode(playerTextureName))
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

// TODO: use direction here
void Player::Shoot(const vec2f & targetPosition)
{
    auto direction = normalize(targetPosition - position);
    float angleFactor = (float)(rand() % 30 - 15) / 100.0f;
    direction = vec2Transform(direction, matrixRotationZ(angleFactor));
    GetGamePtr()->GetEntityMgr()->CreateProjectile(position, direction * 2.0f, 1.0f, 3.0, this);
}

void Player::ShootAlternative(const MathLib::vec2f & targetPosition)
{
    auto direction = normalize(targetPosition - position);

    for (float i = -0.15f; i < 0.25f; i += 0.15f)
    {
        auto new_direction = vec2Transform(direction, matrixRotationZ(i));
        
        // TODO: fix and use direction instead of shoot position
        Shoot(position + new_direction);
    }
}

void Player::SetMoveVelocity(const vec2f & velocity)
{
    moveVelocity = velocity;
}

void Player::Update(double dt)
{
    position += moveVelocity * dt;
    UpdateGraphicsSpritePosition();
}
