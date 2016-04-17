#pragma once

#include <memory>

struct Game
{
    Game();
    ~Game();

    Game(const Game & ref) = delete;
    Game & operator=(const Game & ref) = delete;
    Game(Game && ref) = delete;
    Game & operator=(Game && ref) = delete;

    // PlayerGameObject * player = nullptr;
    // std::unique_ptr<GameObjectsManager> entityMgr = nullptr;
    // std::unique_ptr<GameEventHandler> gameEventHandler = nullptr;
    // std::unique_ptr<GameHUD> gameHud = nullptr;

};

namespace LostIsland
{
    void CreateGame();
    void TerminateGame();

    Game * GetGamePtr();
}
