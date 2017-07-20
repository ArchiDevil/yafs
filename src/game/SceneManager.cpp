#include "SceneManager.h"

#include "../3rdparty/json/json.hpp"
#include "Utilities/logger.hpp"
#include "MathLib/math/vector2.h"
#include "Game.h"
#include "Entities/Player.h"
#include "Entities/EntityManager.h"

#include <fstream>
#include <map>
#include <codecvt>

using namespace nlohmann;

void SceneManager::LoadScene(std::string sceneName)
{
    // TODO Clean EntitiesManager and SceneGraph.

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

    if (jRoot.count("map") && !jRoot["map"].empty())
    {
        std::map<char, std::string> tiles;

        auto jTilesArray = jRoot["map"]["tiles"];
        for (json::iterator it = jTilesArray.begin(); it != jTilesArray.end(); ++it)
        {
            auto jTile = it.value();
            const std::string s = jTile["symbol"];
            const std::string t = jTile["tile"];
            tiles[s[0]] = t;
        }

        const std::string mapFilePath = jRoot["map"]["file"];

        std::string mapFullPath = "resources/scenes/" + mapFilePath;
        std::ifstream mapStream(mapFullPath);
        if (mapStream.fail() || mapStream.bad())
        {
            LOG_ERROR("Unable to open ", mapFullPath);
            return;
        }

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        float y = 0.0f;
        std::string line;
        while (getline(mapStream, line))
        {
            float x = 0.0f;
            for (char symbol : line)
            {
                std::wstring tilePath = converter.from_bytes(tiles[symbol]);
                ShiftEngine::SpriteSceneNode * sprite = ShiftEngine::GetSceneGraph()->AddSpriteNode(tilePath, ShiftEngine::SL_Floor);
                if (sprite != nullptr) {
                    sprite->SetDrawingMode(ShiftEngine::SpriteSceneNode::SpriteDrawingMode::Additive);
                    sprite->SetLocalPosition({ x, y, 0.0 });
                }
                ++x;
            }
            ++y;
        }
    }
}
