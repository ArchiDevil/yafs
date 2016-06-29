#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"
#include "BackgroundEntity.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <MathLib/math.h>

#include <memory>

class EntityFactory final
{
public:
    EntityFactory() = default;
    ~EntityFactory() = default;

    template<typename T, typename ... Args>
    std::shared_ptr<T> CreateEntity(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
};