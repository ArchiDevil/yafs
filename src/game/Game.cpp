#include "game.h"

//#include "Entities/PlayerGameObject.h"
//#include "Entities/GameObjectsManager.h"
//#include "GameEventHandler.h"
//#include "UI/gameHud.h"

namespace GoingHome
{

Game::Game()
{
    entityMgr.reset(new EntityManager());
    player = entityMgr->CreatePlayer({}, 10.0f).get();
    backgroundMgr.reset(new BackgroundManager(entityMgr.get()));
    //gameEventHandler.reset(new GameEventHandler());
}

Player * Game::GetPlayerPtr()
{
    return player;
}

EntityManager * Game::GetEntityMgr()
{
    return entityMgr.get();
}

BackgroundManager * Game::GetBackgroundMgr()
{
    return backgroundMgr.get();
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
