#include "Renderer.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include "ShiftEngine.h"

#include "SceneGraph/CameraSceneNode.h"

#include <sstream>
#include <cassert>
#include <array>

using namespace ShiftEngine;
using namespace MathLib;

Renderer::Renderer()
{
    FPSTimer.start();
    FPSTimer.tick();
}

void Renderer::DrawAll(RenderQueue & rq, double /*dt*/)
{
    static double baseTime = 0.0f;
    if (FPSTimer.get_running_time() - baseTime >= 1.0f)
    {
        baseTime += 1.0f;
        FPS = FPSCounter;
        millisecondsPerFrame = 1000.0f / FPSCounter;
        FPSCounter = 0;
    }

    //  PreProcess();
    Process(rq);
    //  PostProcess();

    FPSCounter++;
}

void Renderer::Process(RenderQueue &rq)
{
    currentState.Reset();
    drawSprites(rq.GetSpriteNodes(), *rq.GetActiveCamera());
}

void Renderer::PreProcess()
{
    throw std::exception("The method or operation is not implemented.");
}

void Renderer::PostProcess()
{
    throw std::exception("The method or operation is not implemented.");
}

size_t Renderer::GetTextureBindings() const
{
    return currentState.textureBindings;
}

size_t Renderer::GetUniformsBindings() const
{
    return currentState.uniformsBindings;
}

size_t Renderer::GetMatricesBindings() const
{
    return currentState.matricesBindings;
}

double Renderer::GetElapsedTime() const
{
    return elapsedTime;
}

size_t Renderer::GetFPS() const
{
    return FPS;
}

void Renderer::drawSprites(SpritesVault & sprites, CameraSceneNode & currentCamera)
{
    struct textureMatrixWithPadding
    {
        // this is 3x3 matrix and this requires 
        // padding (16 bytes alignment) for HLSL shader
        alignas(16) float firstRow[3];
        alignas(16) float secondRow[3];
        alignas(16) float thirdRow[3];
    };

    auto spriteSortFunctor = [] (SpriteSceneNode* left, SpriteSceneNode* right) { return left->GetDrawingMode() < right->GetDrawingMode(); };
    IContextManager* contextMgr = GetContextManager();

    if (!spriteProgram || !spriteMesh)
        loadSpritesPrerequisites();

    contextMgr->SetZState(true);
    contextMgr->SetRasterizerState(RasterizerState::NoCulling);
    currentState.shaderChanges++;
    currentState.shaderChanged = true;

    GetContextManager()->SetUserDebugEventBegin(L"Sprites drawing");

    for (auto& spriteLayerPair : sprites)
    {
        std::sort(spriteLayerPair.second.begin(), spriteLayerPair.second.end(), spriteSortFunctor);
        contextMgr->SetBlendingState(BlendingState::AlphaEnabled);

        for (const SpriteSceneNode* sprite : spriteLayerPair.second)
        {
            if (!sprite)
            {
                LOG_ERROR("Empty sprite node in render queue");
                continue;
            }

            if (contextMgr->GetBlendingState() == BlendingState::AlphaEnabled && sprite->GetDrawingMode() == SpriteSceneNode::SpriteDrawingMode::Additive)
                contextMgr->SetBlendingState(BlendingState::Additive);

            const ITexturePtr & texture = sprite->GetTexture();

            if (!texture)
                return;

            const vec4f & maskColor = sprite->GetMaskColor();
            const mat4f & matWorld = sprite->GetWorldMatrix();
            mat4f matResult = currentCamera.GetViewMatrix() * currentCamera.GetProjectionMatrix();
            matResult = matWorld * matResult;

            spriteProgram->SetMatrixConstantByName("WVPMatrix", (float*)matResult);
            currentState.matricesBindings++;

            //TODO: ugly-ugly shit, need to do something with it D:<
            textureMatrixWithPadding texMatrix;
            memcpy(texMatrix.firstRow, sprite->GetTextureMatrix()[0], sizeof(float) * 3);
            memcpy(texMatrix.secondRow, sprite->GetTextureMatrix()[1], sizeof(float) * 3);
            memcpy(texMatrix.thirdRow, sprite->GetTextureMatrix()[2], sizeof(float) * 3);

            spriteProgram->SetMatrixConstantByName("TextureMatrix", (float*)&texMatrix);
            currentState.matricesBindings++;
            spriteProgram->SetTextureByIndex(0, texture); // there is only one texture for sprites
            currentState.textureBindings++;
            spriteProgram->SetVectorConstantByName("MaskColor", maskColor.ptr());
            currentState.uniformsBindings++;
            spriteProgram->Apply(currentState.shaderChanged);
            currentState.shaderChanged = false;
            currentState.polygonsCount += GetContextManager()->DrawMesh(spriteMesh);
            currentState.drawCalls++;
        }
    }

    GetContextManager()->SetUserDebugEventEnd();
}

void Renderer::loadSpritesPrerequisites()
{
    if (!spriteMesh)
    {
        std::array<PlainSpriteVertex, 4> ver = {};
        ver[0] = {{-0.5f, -0.5f}, {0.0f, 0.0f}};
        ver[1] = {{0.5f, -0.5f}, {1.0f, 0.0f}};
        ver[2] = {{-0.5f, 0.5f}, {0.0f, 1.0f}};
        ver[3] = {{0.5f, 0.5f}, {1.0f, 1.0f}};

        std::vector<uint32_t> ind = {0, 1, 2, 1, 3, 2};

        spriteMesh = pMeshManager->CreateMeshFromVertices((uint8_t*)ver.data(),
                                                          ver.size() * sizeof(PlainSpriteVertex),
                                                          ind,
                                                          {});
    }
}

double Renderer::GetFrameDuration() const
{
    return millisecondsPerFrame;
}

size_t Renderer::GetDrawCalls() const
{
    return currentState.drawCalls;
}

size_t Renderer::GetDrawnPolygonsCount() const
{
    return currentState.polygonsCount;
}

unsigned int Renderer::GetShaderChanges() const
{
    return currentState.shaderChanges;
}
