#pragma once

#include "../Game.h"
#include "LiveEntity.h"
#include "VisualEntity.h"

class LiveEntity;
class VisualEntity;

class IBuff
{
public:
    virtual ~IBuff() = default;

    // this function returns value from 0.0 to +inf 
    // that shows how much damage should be reduced or increased
    virtual float GetDamageModificationRatio() const = 0;

    virtual void OnActivation(LiveEntity * owner) = 0;
    virtual void OnDeactivation(LiveEntity * owner) = 0;
};

class ShieldBuff final 
    : public IBuff
{
public:
    ~ShieldBuff() = default;

    float GetDamageModificationRatio() const override;
    void OnActivation(LiveEntity * owner) override;
    void OnDeactivation(LiveEntity * owner) override;

private:
    bool active = false;
    std::shared_ptr<VisualEntity> shieldEntity = nullptr;

};
