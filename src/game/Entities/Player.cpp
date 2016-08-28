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
    : LiveEntity(position, health, playerTextureName)
{
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
