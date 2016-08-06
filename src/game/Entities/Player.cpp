#include "../Game.h"
#include "Player.h"
#include "Projectile.h"
#include "EntityFactory.h"

#include <cmath>
#include <GraphicsEngine/ShiftEngine.h>

const std::wstring playerTextureName = L"player_sprite.png";

Player::Player(const MathLib::vec2f & position) 
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(playerTextureName))
{
}

bool Player::handleEvent(const ProjectilePositionEvent & event)
{
    if (CalculateCollision(*event.projectile))
    {
        Kill();
        event.projectile->Kill();
    }

    return true;
}

void Player::Shoot(const MathLib::vec2f & targetPosition)
{
    auto vec = MathLib::normalize(targetPosition - position);
    GoingHome::GetGamePtr()->entityMgr->CreateProjectile(position, vec);
}

void Player::SetMoveVelocity(const MathLib::vec2f & velocity)
{
    moveVelocity = velocity;
}

void Player::Update(double dt)
{
    position += moveVelocity * dt;
    SetSpritePosition();
}
