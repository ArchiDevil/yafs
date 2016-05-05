#pragma once

#include <Utilities/timer.h>
#include <Utilities/IManager.h>

#include "VertexTypes.h"
#include "RenderQueue.h"
#include "IShaderManager.h"

#ifdef D3D11_RENDER
#   include "APIs/D3D11/D3D11ContextManager.h"
#elif OPENGL_RENDER
#   include "APIs/OGL33/OpenGLContextManager.h"
#endif

#include <vector>
#include <unordered_map>

namespace ShiftEngine
{

class Renderer : public IManager
{
    struct RendererState
    {
        void Reset()
        {
            drawCalls = 0;
            polygonsCount = 0;
            shaderChanges = 0;
            textureBindings = 0;
            uniformsBindings = 0;
            matricesBindings = 0;
            shaderChanged = true;
            materialChanged = true;
            currentProgram = nullptr;

            texturesCache.clear();
        }

        size_t drawCalls = 0;
        size_t polygonsCount = 0;
        size_t shaderChanges = 0;
        size_t textureBindings = 0;
        size_t uniformsBindings = 0;
        size_t matricesBindings = 0;

        bool shaderChanged = true;
        bool materialChanged = true;

        std::unordered_map<engineTextures, ITexturePtr> texturesCache;

        IProgramPtr currentProgram = nullptr;
    };

public:
    Renderer(IShaderManager * _pShaderManager);

    void DrawAll(RenderQueue & rq, double dt);

    size_t GetDrawCalls() const;
    size_t GetDrawnPolygonsCount() const;
    size_t GetTextureBindings() const;
    size_t GetUniformsBindings() const;
    size_t GetMatricesBindings() const;

    double GetElapsedTime() const;
    size_t GetFPS() const;
    double GetFrameDuration() const;
    unsigned int GetShaderChanges() const;

private:
    void PreProcess();
    void Process(RenderQueue & rq);
    void PostProcess();

    void bindEngineUniforms(MeshSceneNode * currentNode, const RenderQueue & list);
    void bindCustomUniforms(MeshSceneNode * currentNode, const RenderQueue & list);

    void drawSky(RenderQueue &rq);
    void drawSprites(SpritesVector & sprites, CameraSceneNode & currentCamera);
    void bindLights(const LightsVector & lv, unsigned int startIndex, unsigned int count, Material * matPtr);

    void loadSpritesPrerequisites();

    RendererState                   currentState;

    // this is not the main purpose of the renderer - to calculate FPS
    windows_high_reference_timer    FPSTimer;
    double                          elapsedTime = 0.0;
    size_t                          FPSCounter = 0;
    size_t                          FPS = 0;
    double                          millisecondsPerFrame = 0.0;

    IShaderManager *                pShaderManager = nullptr;
    IProgramPtr                     spriteProgram = nullptr;
    IMeshDataPtr                    spriteMesh = nullptr;
};

}
