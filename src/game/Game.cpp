#include "game.h"

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "BackgroundManager.h"
#include "Spells/SpellsDatabase.h"

//#include "Entities/PlayerGameObject.h"
//#include "Entities/GameObjectsManager.h"
//#include "GameEventHandler.h"
//#include "UI/gameHud.h"

namespace GoingHome
{

Game::Game()
{
    physicsMgr.reset(new Physics::PhysicsManager());
    entityMgr.reset(new EntityManager(physicsMgr.get()));
    player = entityMgr->CreatePlayer({}, 10.0f).get();
    backgroundMgr.reset(new BackgroundManager(entityMgr.get()));
    spellsDatabase.reset(new SpellsDatabase());
    spellsDatabase->LoadSpellsFromFile("resources/gamedata/spells/test_spells.json");
    //gameEventHandler.reset(new GameEventHandler());
}

Player * Game::GetPlayerPtr() const
{
    return player;
}

EntityManager * Game::GetEntityMgr() const
{
    return entityMgr.get();
}

BackgroundManager * Game::GetBackgroundMgr() const
{
    return backgroundMgr.get();
}

SpellsDatabase* Game::GetSpellsDatabase() const
{
    return spellsDatabase.get();
}

Physics::PhysicsManager* Game::GetPhysicsMgr() const
{
    return physicsMgr.get();
}

static Game * GamePtr = nullptr;

void GoingHome::CreateGame()
{
    delete GamePtr;
    GamePtr = new Game();
}

void GoingHome::TerminateGame()
{
    delete GamePtr;
    GamePtr = nullptr;
}

Game * GoingHome::GetGamePtr()
{
    return GamePtr;
}

}
