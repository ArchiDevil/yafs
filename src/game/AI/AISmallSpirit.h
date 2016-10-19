#pragma once
#include "AIBase.h"

class AISmallSpirit : public AIBase
{
public:
    AISmallSpirit();
    virtual ~AISmallSpirit() = default;
    void Update(double dt, LiveEntity * entity) override;

};