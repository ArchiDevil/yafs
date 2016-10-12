#pragma once

#include "game\Entities\LiveEntity.h"

#include <memory>

class AIAction
{
public:
    AIAction();
    virtual ~AIAction() = default;
    virtual void Do(LiveEntity & entity) = 0;
};