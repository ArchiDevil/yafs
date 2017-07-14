#include "ShiftEngine.h"

#ifdef D3D11_RENDER
#include "APIs/D3D11/D3D11ContextManager.h"
#endif

#include <Utilities/ut.h>

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

std::vector<std::string> GetDevicesDescription()
{
    std::vector<std::string> output;

    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    // Create a DirectX graphics interface factory.
    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

    UINT i = 0;
    while (factory->EnumAdapters(i++, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC adapterDesc;
        adapter->GetDesc(&adapterDesc);
        std::wstring buffer = adapterDesc.Description;
        output.push_back(utils::narrow(buffer + L" " + std::to_wstring(adapterDesc.DedicatedVideoMemory)));
    }

    return output;
}

}
