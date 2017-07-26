#pragma once

#include "IContextManager.h"
#include "SceneGraph/SceneGraph.h"
#include "MiscTypes.h"

#include <Windows.h>
#include <vector>
#include <string>

namespace ShiftEngine
{

//TODO: remove window handle and replace it by something cross-platform
bool InitEngine(const GraphicEngineSettings & settings, const PathSettings & paths, HWND hwnd);
void ShutdownEngine();

std::vector<std::string> GetDevicesDescription();

IContextManager * GetContextManager();
SceneGraph * GetSceneGraph();

}
