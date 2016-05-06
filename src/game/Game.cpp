#include "game.h"

//#include "Entities/PlayerGameObject.h"
//#include "Entities/GameObjectsManager.h"
//#include "GameEventHandler.h"
//#include "UI/gameHud.h"

Game::Game()
{
    entityMgr.reset(new EntityManager());
    player = entityMgr->CreatePlayer(MathLib::vec2f(0.0, 0.0));
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
