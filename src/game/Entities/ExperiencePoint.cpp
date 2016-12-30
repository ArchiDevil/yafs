#include "ExperiencePoint.h"

#include "EntityEventManager.h"
#include "LiveEntity.h"

#include <GraphicsEngine/ShiftEngine.h>

using namespace ShiftEngine;

const std::wstring experienceTextureName = L"experience_sprite.png";

ExperiencePoint::ExperiencePoint(MathLib::vec2f position,
                                 int expCount,
                                 const std::shared_ptr<Physics::Entity>& physicsEntity)
    : Entity(position, GetSceneGraph()->AddSpriteNode(experienceTextureName))
    , IPhysicsEntityHolder(physicsEntity)
    , expCount(expCount)
{
    sprite->SetLocalScale(0.75f);
}

void ExperiencePoint::Update(double dt)
{
    remainingTime -= dt;
    if (remainingTime < 0.0)
        Kill();
}

int ExperiencePoint::GetExperienceCount() const
{
    return expCount;
}

void ExperiencePoint::OnCollision(IPhysicsEntityHolder* holder)
{
    ExperiencePoint* expPointCollider = dynamic_cast<ExperiencePoint*>(holder);
    if (expPointCollider)
        return;

    LiveEntity *collider = dynamic_cast<LiveEntity*>(holder);
    if (collider)
    {
        collider->AddExperience(expCount);
        Entity::Kill();
    }
}

