#pragma once
#include "AIAction.h"


class AIActionEscape : public AIAction
{
public:
    AIActionEscape();
    virtual ~AIActionEscape() = default;
    void Do(std::shared_ptr<LiveEntity> entity) override;
};
