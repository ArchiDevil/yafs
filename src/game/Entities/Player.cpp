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

Player::Player(vec2f position,
               float health,
               const std::shared_ptr<Physics::Entity>& physicsEntity)
    : LiveEntity(position, health, playerTextureName, 0, physicsEntity)
{
}

void Player::SetMoveVector(const vec2f & vector)
{
    moveVector = vector;
}

void Player::Update(double dt)
{
    physicsEntity->SetVelocity(moveVector);
    LiveEntity::Update(dt);
}
