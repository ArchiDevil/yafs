#pragma once
#include "AIAction.h"


class AIActionEscape : public AIAction
{
public:
    AIActionEscape();
    virtual ~AIActionEscape() = default;
    void Do(LiveEntity & entity) override;
};
