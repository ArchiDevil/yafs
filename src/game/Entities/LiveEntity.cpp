#include "LiveEntity.h"

#include "../Game.h"
#include "Projectile.h"
#include "ExperiencePoint.h"
#include "Buffs.h"

using namespace MathLib;
using namespace GoingHome;
using namespace ShiftEngine;

LiveEntity::LiveEntity(const vec2f & position, float health, const std::wstring & textureName, int expCount)
    : Entity(position, GetSceneGraph()->AddSpriteNode(textureName))
    , health(health)
    , experienceCount(expCount)
{
}

bool LiveEntity::handleEvent(const ProjectilePositionEvent & event)
{
    if (event.projectile->GetProducer() == this)
        return true;

    if (CalculateCollision(*event.projectile))
    {
        health -= event.projectile->GetDamage();
        event.projectile->Kill();

        if (health <= 0)
        {
            Kill();
            GetGamePtr()->GetEntityMgr()->CreateExperiencePoint(position, experienceCount);
        }
    }

    return true;
}

bool LiveEntity::handleEvent(const ExperiencePointPositionEvent & event)
{
    if (CalculateCollision(*event.expPoint))
    {
        experienceCount += event.expPoint->GetExperienceCount();
        event.expPoint->Kill();
        return false;
    }

    return true;
}

MathLib::vec2f LiveEntity::GetTargetDirection() const
{
    return targetDirection;
}

void LiveEntity::SetTargetDirection(const MathLib::vec2f & val)
{
    targetDirection = val;
}

void LiveEntity::Update(double dt)
{
    for (auto& controller : controllers)
        if (controller)
            controller->Update(dt);
}

void LiveEntity::StartSpellInSlot(ControllerSlot slot)
{
    if (controllers[slot])
        controllers[slot]->SpellKeyDown();
}

void LiveEntity::StopSpellInSlot(ControllerSlot slot)
{
    if (controllers[slot])
        controllers[slot]->SpellKeyUp();
}

ISpellController* LiveEntity::GetSpellController(ControllerSlot slot) const
{
    return controllers[slot].get();
}

float LiveEntity::CalculateDamage(float damage)
{
    float damageModifier = 0.0f;
    for (auto & buff : buffs)
        damageModifier += buff->GetDamageModificationRatio();

    return damage * damageModifier;
}

void LiveEntity::SetSpellController(std::unique_ptr<ISpellController> && controller, ControllerSlot slot)
{
    controllers[slot] = std::move(controller);
}

int LiveEntity::GetExperienceCount()
{
    return experienceCount;
}

void LiveEntity::AddBuff(const std::shared_ptr<IBuff> & buff)
{
    buffs.push_back(buff);
    buff->OnActivation(this);
}

void LiveEntity::RemoveBuff(const std::shared_ptr<IBuff> & buff)
{
    buff->OnDeactivation(this);
    buffs.erase(std::find(buffs.begin(), buffs.end(), buff));
}
