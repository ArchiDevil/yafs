#pragma once

#include "Entity.h"
#include "../Spells/ISpellController.h"

#include <array>
#include <vector>

class IBuff;
class ISpellController;

class LiveEntity
    : public Entity
    , observer<ExperiencePointPositionEvent>
    , observer<ExplosionEvent>
    , observer<ProjectilePositionEvent>
{
public:
    enum Faction
    {
        FactionPlayer,
        FactionEnemy
    };

    enum ControllerSlot
    {
        CS_MainSlot,
        CS_AdditionalSlot,
        CS_ShieldSlot,
        CS_MineSlot,

        CS_Count // must be last
    };

    LiveEntity(const MathLib::vec2f & position, float health, const std::wstring & textureName, int expCount, Faction fact = FactionEnemy);
    virtual ~LiveEntity() = default;

    virtual void    Update(double dt) override;

    void            StartSpellInSlot(ControllerSlot slot);
    void            StopSpellInSlot(ControllerSlot slot);

    bool            observer<ProjectilePositionEvent>::handleEvent(const ProjectilePositionEvent & event) override;
    bool            observer<ExperiencePointPositionEvent>::handleEvent(const ExperiencePointPositionEvent & event) override;
    bool            observer<ExplosionEvent>::handleEvent(const ExplosionEvent & event) override;

    MathLib::vec2f  GetTargetDirection() const;
    void            SetTargetDirection(const MathLib::vec2f & val);
    int             GetExperienceCount() const;
    float           GetMaxHealth() const;
    float           GetHealth() const;
    Faction         GetFaction() const;

    // buff system is TBD but this is just for spells task purposes
    void            AddBuff(const std::shared_ptr<IBuff> & buff);
    void            RemoveBuff(const std::shared_ptr<IBuff> & buff);

    void            SetSpellController(std::unique_ptr<ISpellController> && controller, ControllerSlot slot);
    ISpellController* GetSpellController(ControllerSlot slot) const;

protected:
    float           CalculateDamage(float damage);

    const float     maxHealth;
    const Faction   faction = FactionEnemy;

    float           health = 1.0f;
    int             experienceCount = 0;
    MathLib::vec2f  targetDirection = {};

    std::array<std::unique_ptr<ISpellController>, CS_Count> controllers; // just two controllers
    std::vector<std::shared_ptr<IBuff>> buffs;

    scoped_subscriber<ExperiencePointPositionEvent> experiencePointSubscriber = {&EntityEventManager::GetInstance(), this};
    scoped_subscriber<ExplosionEvent> explosionSubscriber = {&EntityEventManager::GetInstance(), this};
    scoped_subscriber<ProjectilePositionEvent> projectileSubscriber = {&EntityEventManager::GetInstance(), this};

};
