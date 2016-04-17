#include "game.h"

//#include "Entities/PlayerGameObject.h"
//#include "Entities/GameObjectsManager.h"
//#include "GameEventHandler.h"
//#include "UI/gameHud.h"

Game::Game()
{
    //entityMgr.reset(new GameObjectsManager());
    //gameEventHandler.reset(new GameEventHandler());
}

Game::~Game()
{
}

static Game * GamePtr = nullptr;

void LostIsland::CreateGame()
{
    delete GamePtr;
    GamePtr = new Game();
}

void LostIsland::TerminateGame()
{
    delete GamePtr;
    GamePtr = nullptr;
}

Game * LostIsland::GetGamePtr()
{
    return GamePtr;
}
