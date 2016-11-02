#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "BackgroundManager.h"
#include "Spells/SpellsDatabase.h"

#include <Utilities/IManager.h>

#include <memory>

namespace GoingHome
{

class Game final : public IManager
{
public:
    Game();
    ~Game() = default;

    Player*             GetPlayerPtr();
    EntityManager*      GetEntityMgr();
    BackgroundManager*  GetBackgroundMgr();
    SpellsDatabase*     GetSpellsDatabase();

private:
    Player*                             player = nullptr;
    std::unique_ptr<EntityManager>      entityMgr = nullptr;
    std::unique_ptr<BackgroundManager>  backgroundMgr = nullptr;
    std::unique_ptr<SpellsDatabase>     spellsDatabase = nullptr;
    // std::unique_ptr<GameEventHandler> gameEventHandler = nullptr;
    // std::unique_ptr<GameHUD> gameHud = nullptr;
};

void CreateGame();
void TerminateGame();
Game * GetGamePtr();

}
