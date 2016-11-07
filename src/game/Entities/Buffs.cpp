#include "Buffs.h"

#include <GraphicsEngine/ShiftEngine.h>

float ShieldBuff::GetDamageModificationRatio() const
{
    if (active)
        return 0.0f;
    else
        return 1.0f;
}

void ShieldBuff::OnActivation(LiveEntity * owner)
{
    if (active)
        return;

    shieldEntity = GoingHome::GetGamePtr()->GetEntityMgr()->CreateVisualStickerEntity(owner,
                                                                                      owner->GetPosition(),
                                                                                      ShiftEngine::GetSceneGraph()->AddSpriteNode(L"shield.png"));
    active = true;
}

void ShieldBuff::OnDeactivation(LiveEntity * /*owner*/)
{
    if (!active)
        return;

    shieldEntity->Kill();
    shieldEntity.reset();
    active = false;
}
