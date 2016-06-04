#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "BackgroundManager.h"

#include <Utilities/IManager.h>

#include <memory>

struct Game final : public IManager
{
    Game();
    ~Game() = default;

    std::weak_ptr<Player> player;
    std::unique_ptr<EntityManager> entityMgr = nullptr;
    std::unique_ptr<BackgroundManager> backgroundMgr = nullptr;
    // std::unique_ptr<GameEventHandler> gameEventHandler = nullptr;
    // std::unique_ptr<GameHUD> gameHud = nullptr;

};

namespace GoingHome
{
    void CreateGame();
    void TerminateGame();

    Game * GetGamePtr();
}
