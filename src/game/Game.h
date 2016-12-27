#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "BackgroundManager.h"
#include "Spells/SpellsDatabase.h"

#include <PhysicsEngine/PhysicsEngine.h>
#include <Utilities/IManager.h>

#include <memory>

namespace GoingHome
{

class Game final : public IManager
{
public:
    Game();
    ~Game() = default;

    Player*                     GetPlayerPtr() const;
    EntityManager*              GetEntityMgr() const;
    BackgroundManager*          GetBackgroundMgr() const;
    SpellsDatabase*             GetSpellsDatabase() const;
    Physics::PhysicsManager*    GetPhysicsMgr() const;

private:
    std::unique_ptr<Physics::PhysicsManager>    physicsMgr;
    Player*                                     player;
    std::unique_ptr<EntityManager>              entityMgr;
    std::unique_ptr<BackgroundManager>          backgroundMgr;
    std::unique_ptr<SpellsDatabase>             spellsDatabase;
    // std::unique_ptr<GameEventHandler>        gameEventHandler;
    // std::unique_ptr<GameHUD>                 gameHud;
};

void CreateGame();
void TerminateGame();
Game * GetGamePtr();

}
