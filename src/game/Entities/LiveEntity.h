#pragma once

#include "Entity.h"
#include "../Spells/ISpellController.h"

#include <array>

class ISpellController;

class LiveEntity : public Entity
{
public:
    enum ControllerSlot
    {
        CS_MainSlot,
        CS_AdditionalSlot,
        CS_ShieldSlot,
        CS_MineSlot,

        CS_Count // must be last
    };

    LiveEntity(const MathLib::vec2f & position, float health, const std::wstring & textureName, int expCount);
    virtual ~LiveEntity() = default;

    virtual void    Update(double dt) override;

    void            StartSpellInSlot(ControllerSlot slot);
    void            StopSpellInSlot(ControllerSlot slot);

    bool            observer<ProjectilePositionEvent>::handleEvent(const ProjectilePositionEvent & event) override;
    bool            observer<ExperiencePointPositionEvent>::handleEvent(const ExperiencePointPositionEvent & event) override;

    MathLib::vec2f  GetTargetDirection() const;
    void            SetTargetDirection(const MathLib::vec2f & val);
    int             GetExperienceCount();

    void SetSpellController(std::unique_ptr<ISpellController> && controller, ControllerSlot slot);
    ISpellController* GetSpellController(ControllerSlot slot) const;

protected:
    float           health = 1.0f;
    int             experienceCount = 0;
    MathLib::vec2f  targetDirection = {};

    std::array<std::unique_ptr<ISpellController>, CS_Count> controllers; // just two controllers

};
