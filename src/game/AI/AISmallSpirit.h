#pragma once
#include "AIBase.h"

class AISmallSpirit : public AIBase
{
public:
    AISmallSpirit();
    virtual ~AISmallSpirit() = default;
    void Update(std::shared_ptr<LiveEntity> entity) override;

};