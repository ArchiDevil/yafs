#pragma once

#include "Entities/EntityManager.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/IManager.h>

#include <vector>

// This class manages backgrounds on different levels
// For now it has only hard coded background for the first level
// TODO: implement system with background loaded from files/scripts
class BackgroundManager : public IManager
{
public:
    BackgroundManager(EntityManager *entityMgr);
    ~BackgroundManager() = default;

    void Update(double dt);

private:
    std::vector<ShiftEngine::EmptySceneNode*> backgroundLayers;

};
