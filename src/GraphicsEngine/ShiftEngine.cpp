#include "ShiftEngine.h"

#ifdef D3D11_RENDER
#include "APIs/D3D11/D3D11ContextManager.h"
#endif

#include <Utilities/ut.h>

namespace ShiftEngine
{

std::unique_ptr<IContextManager> ContextManagerInstance;
std::unique_ptr<SceneGraph> SceneGraphInstance;

bool InitDX11Api(HWND hwnd, GraphicEngineSettings settings, PathSettings paths)
{
    ContextManagerInstance = std::make_unique<ShiftEngine::D3D11ContextManager>(hwnd, settings, paths);
    SceneGraphInstance = std::make_unique<ShiftEngine::SceneGraph>();
    return true;
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
