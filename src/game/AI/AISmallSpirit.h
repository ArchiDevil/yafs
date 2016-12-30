#pragma once

#include "AIBase.h"

class AISmallSpirit : public AIBase
{
public:
    virtual ~AISmallSpirit() = default;
    void Update(double dt, LiveEntity * entity) override;

};
