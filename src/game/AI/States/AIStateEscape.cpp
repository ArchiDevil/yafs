#include "AIStateEscape.h"

#include "../../Game.h"

AIStateEscape::AIStateEscape()
    : AIState()
{

}

void AIStateEscape::Do(double dt, LiveEntity * entity)
{
    auto entPosition = entity->GetPosition();

    static double elapsedTime = 0.0;
    elapsedTime += dt;

    if (elapsedTime > 0.5) {
        runVector = { 0.0f, 0.0f };
        const std::vector<std::shared_ptr<LiveEntity>> * hostileEntList = GoingHome::GetGamePtr()->GetEntityMgr()->GetHostileLiveEntities(entity->GetFraction());
        for (auto it = (*hostileEntList).begin(); it != (*hostileEntList).end(); ++it)
        {
            if (4.0f > MathLib::distance(entPosition, (*it)->GetPosition()))
                runVector += entPosition - (*it)->GetPosition();
        }
        elapsedTime = 0.0;
    }

    if (runVector.length() != 0.0f)
    {
        entPosition += MathLib::normalize(runVector) * (dt / 2);
        entity->SetPosition(entPosition);
        entity->UpdateGraphicsSpritePosition();
    }
}
