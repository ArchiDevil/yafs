#include "ExperiencePoint.h"

#include "EntityEventManager.h"

#include <GraphicsEngine/ShiftEngine.h>

using namespace ShiftEngine;

const std::wstring experienceTextureName = L"experience_sprite.png";

ExperiencePoint::ExperiencePoint(const MathLib::vec2f & position, int expCount)
    : Entity(position, GetSceneGraph()->AddSpriteNode(experienceTextureName))
    , expCount(expCount)
{
    sprite->SetLocalScale(0.75f);
}

void ExperiencePoint::Update(double dt)
{
    remainingTime -= dt;
    if (remainingTime < 0.0)
    {
        Kill();
        return;
    }

    ((notifier<ExperiencePointPositionEvent>)EntityEventManager::GetInstance())
        .notifyAll(ExperiencePointPositionEvent(this));
}

int ExperiencePoint::GetExperienceCount() const
{
    return expCount;
}

