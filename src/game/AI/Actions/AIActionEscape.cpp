#include "AIActionEscape.h"

#include "../../Game.h"

AIActionEscape::AIActionEscape()
    : AIAction()
{

}

void AIActionEscape::Do(double dt, LiveEntity * entity)
{
    const std::vector<std::shared_ptr<LiveEntity>> * hostileEntList = GoingHome::GetGamePtr()->GetEntityMgr()->GetHostileLiveEntities(entity->GetFraction());
    MathLib::vec2f runVector = { 0, 0 };
    auto entPosition = entity->GetPosition();
    for (auto it = (*hostileEntList).begin(); it != (*hostileEntList).end(); ++it)
    {
        runVector += MathLib::normalize((*it)->GetPosition() - entPosition);
    }

    entPosition += runVector * dt;
    entity->SetPosition(entPosition);
    entity->UpdateGraphicsSpritePosition();
}
