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
