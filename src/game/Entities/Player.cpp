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
    : LiveEntity(position, health, playerTextureName, 0)
{
}

void Player::ShootAlternative(const MathLib::vec2f & direction)
{
    for (float i = -0.15f; i < 0.25f; i += 0.15f)
        Shoot(vec2Transform(direction, matrixRotationZ(i)));
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
