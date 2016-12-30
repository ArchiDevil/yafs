#pragma once

#include "AIBase.h"

class AISmallSpirit : public AIBase
{
public:
    AISmallSpirit(float lowHealthThresholdRation);

    virtual ~AISmallSpirit() = default;
    void Update(double dt, LiveEntity * entity) override;

private:
    float lowHealthThresholdRation = 0.3f;

};
