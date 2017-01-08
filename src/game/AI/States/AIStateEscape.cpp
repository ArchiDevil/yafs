#include "AIStateEscape.h"

#include "../../Game.h"

using namespace GoingHome;
using namespace MathLib;

AIStateEscape::AIStateEscape(float escapeDistance)
    : escapeDistance(escapeDistance)
{
}

void AIStateEscape::Do(double dt, LiveEntity * entity)
{
    auto position = entity->GetPosition();

    static double elapsedTime = 0.0;
    elapsedTime += dt;

    if (elapsedTime > 0.5)
    {
        runVector = {};
        for (auto& hostileEntity : GetGamePtr()->GetEntityMgr()->GetHostileLiveEntities(entity->GetFaction()))
            if (distance(position, hostileEntity->GetPosition()) < escapeDistance)
                runVector += position - hostileEntity->GetPosition();

        elapsedTime = 0.0;
    }

    if (runVector.length())
    {
        position += normalize(runVector) * (dt / 2.0);
        entity->SetPosition(position);
    }
}
