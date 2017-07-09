#include "ShiftEngine.h"

#ifdef D3D11_RENDER
#include "APIs/D3D11/D3D11ContextManager.h"
#endif

namespace ShiftEngine
{

std::unique_ptr<IContextManager> ContextManagerInstance;
std::unique_ptr<SceneGraph> SceneGraphInstance;

bool InitDX11Api(HWND hwnd, GraphicEngineSettings settings, PathSettings paths, SceneGraphType sceneGraphType)
{
    ContextManagerInstance.reset(new D3D11ContextManager(hwnd));
    if (!ContextManagerInstance->Initialize(settings, paths))
        return false;
    SceneGraphInstance = std::make_unique<ShiftEngine::SceneGraph>(sceneGraphType);
    return true;
}

bool InitOpenGLApi(HWND /*hwnd*/, GraphicEngineSettings /*settings*/, PathSettings /*paths*/, SceneGraphType /*sceneGraphType*/)
{
    return false;
}

bool InitEngine(const GraphicEngineSettings & settings, const PathSettings & paths, HWND hwnd, SceneGraphType sceneGraphType)
{
#if defined(D3D11_RENDER)
    return InitDX11Api(hwnd, settings, paths, sceneGraphType);
#elif defined(OPENGL_RENDER)
    return InitOpenGLApi(hwnd, settings, paths, sceneGraphType);
#endif;
}

IContextManager * GetContextManager()
{
    return ContextManagerInstance.get();
}

SceneGraph * GetSceneGraph()
{
    return SceneGraphInstance.get();
}

void ShutdownEngine()
{
    SceneGraphInstance.reset();
    ContextManagerInstance.reset();
}

}
