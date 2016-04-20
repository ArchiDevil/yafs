#include "ShiftEngine.h"

#ifdef D3D11_RENDER
#include "APIs/D3D11/D3D11ContextManager.h"
#endif

namespace ShiftEngine
{
    std::unique_ptr<IContextManager> ContextManagerInstance;
    std::unique_ptr<SceneGraph> SceneGraphInstance;
    std::unique_ptr<Renderer> RendererInstance;

    bool InitDX11Api(HWND hwnd, GraphicEngineSettings settings, PathSettings paths)
    {
#ifdef D3D11_RENDER
        ContextManagerInstance.reset(new D3D11ContextManager(hwnd));
        if (!ContextManagerInstance->Initialize(settings, paths))
            return false;
        RendererInstance.reset(new Renderer(ContextManagerInstance->GetShaderManager(), ContextManagerInstance->GetShaderGenerator()));
        SceneGraphInstance.reset(new SceneGraph(SceneGraphType::SGT_QuadTree));
        return true;
#else
        return false;
#endif
    }

    bool InitOpenGLApi(HWND /*hwnd*/, GraphicEngineSettings /*settings*/, PathSettings /*paths*/)
    {
        return false;
    }

    bool InitEngine(const GraphicEngineSettings & settings, const PathSettings & paths, HWND hwnd)
    {
#if defined(D3D11_RENDER)
        return InitDX11Api(hwnd, settings, paths);
#elif defined(OPENGL_RENDER)
        return InitOpenGLApi(hwnd, settings, paths);
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

    Renderer * GetRenderer()
    {
        return RendererInstance.get();
    }

    void ShutdownEngine()
    {
        SceneGraphInstance.reset(nullptr);
        RendererInstance.reset(nullptr);
        ContextManagerInstance.reset(nullptr);
    }
}
