#include "Renderer.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include "ShiftEngine.h"

#include "SceneGraph/CameraSceneNode.h"
#include "SceneGraph/LightSceneNode.h"

#include <sstream>
#include <cassert>
#include <array>

using namespace ShiftEngine;
using namespace MathLib;

Renderer::Renderer(IShaderManager * _pShaderManager)
    : pShaderManager(_pShaderManager)
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

void Renderer::drawSky(RenderQueue &rq)
{
    SkySceneNode * skyNode = rq.GetActiveSky();
    if (skyNode)
    {
        currentState.shaderChanges++;
        auto pos = rq.GetActiveCamera()->GetWorldPosition();
        skyNode->SetLocalPosition(MathLib::vec3f(pos.x, pos.y, pos.z));
        skyNode->SetLocalScale(MathLib::vec3f(10.0f, 10.0f, 10.0f));
        GetContextManager()->SetZState(false);

        Material * mat = skyNode->GetMaterialPtr();
        currentState.currentProgram = mat->program;

        bindEngineUniforms(skyNode, rq);
        bindCustomUniforms(skyNode, rq);

        currentState.currentProgram->Apply(true);
        currentState.polygonsCount += skyNode->Render();
        currentState.drawCalls++;
    }
}

bool sortFunctor(MeshSceneNode * left, MeshSceneNode * right)
{
    bool leftTr = left->GetMaterialPtr()->GetMaterialInfo()->GetFlags()->isTransparent;
    bool rightTr = right->GetMaterialPtr()->GetMaterialInfo()->GetFlags()->isTransparent;
    if (!leftTr && rightTr)
        return true;
    else
        return false;
}

void Renderer::Process(RenderQueue &rq)
{
    currentState.Reset();

    //sky first
    drawSky(rq);
    auto & renderVec = rq.GetRenderableNodes();

    std::sort(renderVec.begin(), renderVec.end(), sortFunctor);

    GetContextManager()->SetZState(true);

    for (auto iter = renderVec.begin(); iter != renderVec.end(); ++iter)
    {
        MeshSceneNode * currentNode = (*iter);
        if (!currentNode->GetDataPtr())
            continue;

        //bind material
        Material * mat = currentNode->GetMaterialPtr();

        if (!mat->program)
        {
            //PANIC COMPILE SHADER!!!
            mat->program = pShaderManager->CreateProgramFromMaterialFlags(mat->info, *currentNode->GetDataPtr()->GetVertexSemantic());
            mat->LinkMaterial();
        }

        if (mat->program != currentState.currentProgram)
        {
            currentState.currentProgram = mat->program;
            currentState.shaderChanged = true;
            currentState.shaderChanges++;
        }

        //check already set textures
        for (int i = 0; i < ST_Elems; i++)
        {
            if (mat->builtinTextures[i].first != UINT_MAX &&
                currentState.texturesCache[(engineTextures)i] != mat->builtinTextures[i].second)
            {
                if (!currentState.currentProgram->SetTextureByIndex(mat->builtinTextures[i].first, mat->builtinTextures[i].second))
                {
                    LOG_ERROR("Unable to set texture for material");
                }
                currentState.texturesCache[(engineTextures)i] = mat->builtinTextures[i].second;
                currentState.textureBindings++;
            }
        }

        bindEngineUniforms(currentNode, rq);
        bindCustomUniforms(currentNode, rq);

        currentState.currentProgram->Apply(currentState.shaderChanged);
        currentState.polygonsCount += currentNode->Render();
        currentState.drawCalls++;

        currentState.shaderChanged = false;
    }

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

void Renderer::bindEngineUniforms(MeshSceneNode * currentNode, const RenderQueue & list)
{
    if (!currentNode || !currentNode->GetMaterialPtr() || !currentNode->GetMaterialPtr()->program)
        return;

    Material * curMaterial = currentNode->GetMaterialPtr();
    const MaterialInfo * curMaterialInfo = curMaterial->GetMaterialInfo();
    IProgramPtr & program = curMaterial->program;

    const auto & uniforms = curMaterial->GetUniforms();
    size_t uniformsNum = uniforms.size();
    for (size_t i = 0; i != uniformsNum; ++i)
    {
        auto uniformId = uniforms[i];
        auto uniformIndex = curMaterial->GetUniformIndex(uniformId);

        switch (uniformId)
        {
        case SV_MatWorld:
            currentState.matricesBindings++;
            program->SetMatrixConstantByIndex(uniformIndex, currentNode->GetWorldMatrix());
            break;
        case SV_MatView:
            if (currentState.shaderChanged)
            {
                currentState.matricesBindings++;
                program->SetMatrixConstantByIndex(uniformIndex, list.GetActiveCamera()->GetViewMatrix());
            }
            break;
        case SV_MatProj:
            if (currentState.shaderChanged)
            {
                currentState.matricesBindings++;
                program->SetMatrixConstantByIndex(uniformIndex, list.GetActiveCamera()->GetProjectionMatrix());
            }
            break;
        case SV_EyePos:
            if (currentState.shaderChanged)
            {
                currentState.uniformsBindings++;
                program->SetVectorConstantByIndex(uniformIndex, list.GetActiveCamera()->GetWorldPosition().ptr());
            }
            break;
        case SV_AmbientColor:
            if (currentState.shaderChanged)
            {
                currentState.uniformsBindings++;
                program->SetVectorConstantByIndex(uniformIndex, list.GetAmbientColor().ptr());
            }
            break;
        case SV_LightsArray:
            if (currentState.shaderChanged)
            {
                currentState.uniformsBindings++;
                bindLights(list.GetLights(), 0, list.GetLights().size(), curMaterial);
            }
            break;
        case SV_LightsCount:
            if (currentState.shaderChanged)
            {
                currentState.uniformsBindings++;
                float lightsCount = (float)list.GetLights().size();
                program->SetScalarConstantByIndex(uniformIndex, &lightsCount);
            }
            break;
        case SV_DiffuseColor:
            currentState.uniformsBindings++;
            program->SetVectorConstantByIndex(uniformIndex, curMaterialInfo->diffuseColor.ptr());
            break;
        case SV_SpecularColor:
            currentState.uniformsBindings++;
            program->SetVectorConstantByIndex(uniformIndex, curMaterialInfo->specularColor.ptr());
            break;
        case SV_EmissionColor:
            currentState.uniformsBindings++;
            program->SetVectorConstantByIndex(uniformIndex, curMaterialInfo->emissionColor.ptr());
            break;
        case SV_Transparency:
            currentState.uniformsBindings++;
            program->SetScalarConstantByIndex(uniformIndex, &curMaterialInfo->opacity);
            break;
        case SV_Shininess:
            currentState.uniformsBindings++;
            program->SetScalarConstantByIndex(uniformIndex, &curMaterialInfo->shininess);
            break;
        default:
            assert(false);
            break;
        }
    }
}

void Renderer::bindCustomUniforms(MeshSceneNode * currentNode, const RenderQueue & /*list*/)
{
    auto mat = currentNode->GetMaterialPtr();

    if (!currentState.shaderChanged)
        return;

    if (!mat->floatParams.empty())
    {
        for (auto & elem : mat->floatParams)
        {
            mat->program->SetScalarConstantByName(elem.first.c_str(), &elem.second);
            currentState.uniformsBindings++;
        }
    }

    if (!mat->float2Params.empty())
    {
        for (auto & elem : mat->float2Params)
        {
            mat->program->SetVectorConstantByName(elem.first.c_str(), elem.second.ptr());
            currentState.uniformsBindings++;
        }
    }

    if (!mat->float3Params.empty())
    {
        for (auto & elem : mat->float3Params)
        {
            mat->program->SetVectorConstantByName(elem.first.c_str(), elem.second.ptr());
            currentState.uniformsBindings++;
        }
    }

    if (!mat->float4Params.empty())
    {
        for (auto & elem : mat->float4Params)
        {
            mat->program->SetVectorConstantByName(elem.first.c_str(), elem.second.ptr());
            currentState.uniformsBindings++;
        }
    }
}

void Renderer::bindLights(const LightsVector & lv, unsigned int startIndex, unsigned int count, Material * matPtr)
{
    if (!matPtr)
        return;

    if (currentState.shaderChanged)
    {
        std::vector<LightInfo> li;
        LightInfo temp;
        for (size_t i = startIndex; i < startIndex + count; i++)
        {
            temp.Type = lv[i]->GetType();
            temp.Color = lv[i]->GetColor();
            temp.Direction = lv[i]->GetDirection();
            temp.Position = lv[i]->GetWorldPosition();
            temp.Radius = lv[i]->GetRadius();
            li.push_back(temp);
        }

        currentState.uniformsBindings += 4 + 1 + 1;

        matPtr->BindLights(li);
    }
}

size_t ShiftEngine::Renderer::GetTextureBindings() const
{
    return currentState.textureBindings;
}

size_t ShiftEngine::Renderer::GetUniformsBindings() const
{
    return currentState.uniformsBindings;
}

size_t ShiftEngine::Renderer::GetMatricesBindings() const
{
    return currentState.matricesBindings;
}

double Renderer::GetElapsedTime() const
{
    return elapsedTime;
}

size_t ShiftEngine::Renderer::GetFPS() const
{
    return FPS;
}

void ShiftEngine::Renderer::drawSprites(SpritesVector & sprites, CameraSceneNode & currentCamera)
{
    if (!spriteProgram)
        spriteProgram = GetContextManager()->LoadShader(L"SpriteShader.fx");

    if (!spriteMesh)
    {
        std::array<PlainSpriteVertex, 4> ver = {};
        ver[0] = {{-0.5f, -0.5f}, {0.0f, 0.0f}};
        ver[1] = {{0.5f, -0.5f}, {1.0f, 0.0f}};
        ver[2] = {{-0.5f, 0.5f}, {0.0f, 1.0f}};
        ver[3] = {{0.5f, 0.5f}, {1.0f, 1.0f}};

        std::vector<uint32_t> ind = {0, 1, 2, 1, 3, 2};

        IMeshManager * pMeshManager = GetContextManager()->GetMeshManager();
        spriteMesh = pMeshManager->CreateMeshFromVertices((uint8_t*)ver.data(),
                                                          ver.size() * sizeof(PlainSpriteVertex),
                                                          ind,
                                                          &plainSpriteVertexSemantic,
                                                          {});
    }

    GetContextManager()->SetZState(true);
    GetContextManager()->SetRasterizerState(RasterizerState::NoCulling);
    currentState.shaderChanges++;
    currentState.currentProgram = spriteProgram;
    for (auto & sprite : sprites)
    {
        auto & texture = sprite->GetTexture();

        if (!texture)
            return;

        const vec3f & position = sprite->GetWorldPosition();
        const vec3f & scale = sprite->GetWorldScale();
        const qaFloat rotation = sprite->GetWorldRotation();
        const vec4f & maskColor = sprite->GetMaskColor();

        mat4f matResult, matScale, matPos, matRot;
        matPos = matrixTranslation(position.x, position.y, 0.0f);
        matScale = matrixScaling(scale.x, scale.y, 0.0f);
        matRot = rotation.to_matrix();
        matResult = currentCamera.GetProjectionMatrix();
        matResult = (matScale * matRot * matPos) * matResult;

        spriteProgram->SetMatrixConstantByName("matRes", (float*)matResult);
        currentState.matricesBindings++;
        spriteProgram->SetTextureByName("Texture", texture);
        currentState.textureBindings++;
        spriteProgram->SetVectorConstantByName("MaskColor", maskColor.ptr());
        currentState.uniformsBindings++;
        spriteProgram->Apply(true);
        currentState.polygonsCount += GetContextManager()->DrawMesh(spriteMesh);
        currentState.drawCalls++;
    }
}

double Renderer::GetFrameDuration() const
{
    return millisecondsPerFrame;
}

size_t ShiftEngine::Renderer::GetDrawCalls() const
{
    return currentState.drawCalls;
}

size_t ShiftEngine::Renderer::GetDrawnPolygonsCount() const
{
    return currentState.polygonsCount;
}

unsigned int Renderer::GetShaderChanges() const
{
    return currentState.shaderChanges;
}
