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

void Player::SetMoveVelocity(const vec2f & velocity)
{
    moveVelocity = velocity;
}

void Player::Update(double dt)
{
    position += moveVelocity * dt;
    UpdateGraphicsSpritePosition();
    LiveEntity::Update(dt);
}
