#include "AIStateEscape.h"

#include "../../Game.h"

void AIStateEscape::Do(double dt, LiveEntity * entity)
{
    auto entPosition = entity->GetPosition();

    static double elapsedTime = 0.0;
    elapsedTime += dt;

    if (elapsedTime > 0.5) 
    {
        runVector = {};
        const std::vector<std::shared_ptr<LiveEntity>> * hostileEntList = GoingHome::GetGamePtr()->GetEntityMgr()->GetHostileLiveEntities(entity->GetFaction());
        for (auto& hostileEntity : *hostileEntList)
        {
            if (4.0f > MathLib::distance(entPosition, hostileEntity->GetPosition()))
                runVector += entPosition - hostileEntity->GetPosition();
        }
        elapsedTime = 0.0;
    }

    if (runVector.length() != 0.0f)
    {
        entPosition += MathLib::normalize(runVector) * (dt / 2.0);
        entity->SetPosition(entPosition);
    }
}
