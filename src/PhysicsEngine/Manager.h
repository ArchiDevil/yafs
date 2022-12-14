#pragma once

#include <Utilities/IManager.h>
#include <MathLib/math.h>

#include "Entity.h"
#include "Types.h"

#include <vector>
#include <memory>

namespace Physics
{

class PhysicsManager
{
public:
    std::shared_ptr<Entity> CreateEntity(const MathLib::vec2f & position, float size, const MathLib::vec2f & speed = {});
    void                    Update(double dt);

private:
    std::vector<std::weak_ptr<Entity>> entities;

};

}
