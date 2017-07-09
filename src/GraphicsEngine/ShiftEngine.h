#pragma once

#include <Windows.h>

#include "IContextManager.h"
#include "SceneGraph/SceneGraph.h"
#include "MiscTypes.h"

namespace ShiftEngine
{
//TODO: remove window handle and replace it by something cross-platform
bool InitEngine(const GraphicEngineSettings & settings, const PathSettings & paths, HWND hwnd, SceneGraphType sceneGraphType);
void ShutdownEngine();

IContextManager * GetContextManager();
SceneGraph * GetSceneGraph();
}
