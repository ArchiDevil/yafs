#pragma once

#include "MineEntity.h"

#include <Utilities/observer.h>

class MineDetectorEntity final
    : public MineEntity
    , observer<LiveEntityPositionEvent>
{
public:
    MineDetectorEntity(const LiveEntity * owner, const MathLib::vec2f& position, float explosionDamage, float explosionRadius, float triggerDistance);
    void Update(double dt) override;
    bool observer<LiveEntityPositionEvent>::handleEvent(const LiveEntityPositionEvent & event) override;

private:
    scoped_subscriber<LiveEntityPositionEvent> liveEntityPositionEvent = {&EntityEventManager::GetInstance(), this};

};
