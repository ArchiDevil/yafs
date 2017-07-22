#pragma once

#include <Utilities/IManager.h>

#include <string>

class LevelManager final : public IManager
{
public:
    static void LoadScene(const std::string& sceneName);

private:
    enum GameObjectType
    {
        SmallSpiritEnemy = 1
    };
};
