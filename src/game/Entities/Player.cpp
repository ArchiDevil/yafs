#include "../Game.h"
#include "Player.h"
#include "Projectile.h"
#include "EntityFactory.h"

#include <cmath>
#include <GraphicsEngine/ShiftEngine.h>

const std::wstring textureName = L"player";

Player::Player(const MathLib::vec2f & position) 
    : Entity(position, ShiftEngine::GetSceneGraph()->AddSpriteNode(textureName))
{
}

bool Player::handleEvent(const ProjectilePositionEvent & event)
{
    if (CalculateCollision(*event.projectile))
        isToDelete = true;
    return true;
}

void Player::Shoot(const MathLib::vec2f & targetPosition)
{
    auto vec = MathLib::normalize<float>(targetPosition - position);

    GoingHome::GetGamePtr()->entityMgr->CreateProjectile(position, vec);
}
