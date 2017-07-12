#include "SceneManager.h"

#include "../3rdparty/json/json.hpp"
#include "Utilities/logger.hpp"
#include "MathLib/math/vector2.h"
#include "Game.h"
#include "Entities/Player.h"
#include "Entities/EntityManager.h"

#include <fstream>

using namespace nlohmann;

void SceneManager::LoadScene(std::string sceneName)
{
    std::string sceneFullPath = "resources/scenes/" + sceneName + ".json";
    std::ifstream jsonfile(sceneFullPath);
    if (jsonfile.fail() || jsonfile.bad())
    {
        LOG_ERROR("Unable to open ", sceneFullPath);
        return;
    }
    json jRoot;
    jsonfile >> jRoot;

    if (jRoot.is_null() || jRoot.empty())
    {
        LOG_ERROR("Can't parse json object ", sceneFullPath);
        return;
    }

    std::vector<MathLib::vec2f> startingPointList;
    if (jRoot.count("starting_point") && !jRoot["starting_point"].empty())
    {
        auto jStartPoint = jRoot["starting_point"];
        for (json::iterator it = jStartPoint.begin(); it != jStartPoint.end(); ++it)
        {
            startingPointList.push_back({ it.value()["x"].get<float>(), it.value()["y"].get<float>() });
        }
    }

    if (!startingPointList.empty())
    {
        Player* playerPtr = GoingHome::GetGamePtr()->GetPlayerPtr();
        playerPtr->SetPosition(startingPointList[0]);
    }

    if (jRoot.count("game_objects") && !jRoot["game_objects"].empty())
    {
        auto jGameObj = jRoot["game_objects"];

        for (json::iterator it = jGameObj.begin(); it != jGameObj.end(); ++it)
        {
            auto gameObj = it.value();
            switch(static_cast<GameObjectType>(gameObj["type"].get<int>()))
            {
            case SmallSpiritEnemy:
                GoingHome::GetGamePtr()->GetEntityMgr()->CreateEnemy(
                    { gameObj["position"]["x"].get<float>(), gameObj["position"]["y"].get<float>() },
                    gameObj["health"].get<float>(),
                    gameObj["experience"].get<int>(),
                    gameObj["size"].get<float>(),
                    Enemy::EnemyType::SmallSpirit);
                break;
            }
        }
    }
}
