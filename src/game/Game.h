#pragma once

#include <PhysicsEngine/PhysicsEngine.h>
#include <Utilities/IManager.h>

#include <memory>

// these forwarding is to force user to include files
// this should make user to think twice if he really needs to use this class
class EntityManager;
class Player;
class BackgroundManager;
class SpellsDatabase;

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
