#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"

#include <memory>

struct Game
{
    Game();
    ~Game();

    Game(const Game & ref) = delete;
    Game & operator=(const Game & ref) = delete;
    Game(Game && ref) = delete;
    Game & operator=(Game && ref) = delete;

    std::weak_ptr<Player> player;
    std::unique_ptr<EntityManager> entityMgr = nullptr;
    // std::unique_ptr<GameEventHandler> gameEventHandler = nullptr;
    // std::unique_ptr<GameHUD> gameHud = nullptr;

};

namespace GoingHome
{
    void CreateGame();
    void TerminateGame();

    Game * GetGamePtr();
}
