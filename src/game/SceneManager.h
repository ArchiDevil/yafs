#pragma once

#include <Utilities/IManager.h>

#include <string>

class SceneManager final : public IManager
{
public:
    static void LoadScene(std::string sceneName);

private:
    enum GameObjectType
    {
        SmallSpiritEnemy = 1
    };
};
