#include "AIActionEscape.h"

#include "../../Game.h"

AIActionEscape::AIActionEscape()
    : AIAction()
{

}

void AIActionEscape::Do(LiveEntity * entity)
{
    auto hostileEnt = GoingHome::GetGamePtr()->GetEntityMgr()->GetHostileLiveEntities(entity->GetFraction());


}
