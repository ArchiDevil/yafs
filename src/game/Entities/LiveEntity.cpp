#include "LiveEntity.h"

#include <GraphicsEngine/ShiftEngine.h>

#include "../Game.h"
#include "EntityManager.h"
#include "Projectile.h"
#include "ExperiencePoint.h"
#include "Buffs.h"

using namespace MathLib;
using namespace GoingHome;
using namespace ShiftEngine;

LiveEntity::LiveEntity(vec2f position,
                       float health,
                       const std::wstring & textureName,
                       int expCount,
                       const std::shared_ptr<Physics::Entity>& physicsEntity,
                       Faction fact)
    : Entity(position, GetSceneGraph()->AddSpriteNode(textureName))
    , IPhysicsEntityHolder(physicsEntity)
    , maxHealth(health)
    , health(health)
    , experienceCount(expCount)
    , faction(fact)
{
}

bool LiveEntity::handleEvent(const ExplosionEvent & event)
{
    if (MathLib::distance(Entity::GetPosition(), event.epicenter) < event.radius)   // radius of entity is not supported
    {                                                                               // if you want to add it, just add it to the radius
        // whoops, duplicate with projectile event
        health -= event.damage;
        if (health <= 0)
            Kill();
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
    Entity::SetPosition(IPhysicsEntityHolder::physicsEntity->GetPosition());

    for (auto& controller : controllers)
        if (controller)
            controller->Update(dt);

    ((notifier<LiveEntityPositionEvent>)EntityEventManager::GetInstance())
        .notifyAll(LiveEntityPositionEvent(this));
}

void LiveEntity::Kill()
{
    Entity::Kill();
    IPhysicsEntityHolder::physicsEntity = nullptr;
    GetGamePtr()->GetEntityMgr()->CreateExperiencePoint(Entity::GetPosition(), experienceCount, 0.2f);
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

void LiveEntity::TakeDamage(float damageCount)
{
    health -= damageCount;

    if (health <= 0)
        Kill();
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

int LiveEntity::GetExperienceCount() const
{
    return experienceCount;
}

void LiveEntity::AddExperience(int expCount)
{
    experienceCount += expCount;
}

float LiveEntity::GetMaxHealth() const
{
    return maxHealth;
}

float LiveEntity::GetHealth() const
{
    return health;
}

LiveEntity::Faction LiveEntity::GetFaction() const
{
    return faction;
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

MathLib::vec2f LiveEntity::GetPosition() const
{
    return IPhysicsEntityHolder::physicsEntity->GetPosition();
}

void LiveEntity::SetPosition(MathLib::vec2f pos)
{
    IPhysicsEntityHolder::physicsEntity->SetPosition(pos);
}
