#include "SceneGraph.h"

#include "../ShiftEngine.h"
#include "../RenderQueue.h"

#include <cassert>
#include <array>

using namespace ShiftEngine;

SceneGraph::SceneGraph(SceneGraphType graphType /*= SGT_Plain*/)
    : type(graphType)
{
    skySemantic.addSemantic(ET_FLOAT, 3, ES_Position);
    GetContextManager()->GetVertexDeclaration(skySemantic);

    switch (graphType)
    {
    case SGT_Plain:
        rootNode = new PlainTreeNode(this);
        break;
    case SGT_QuadTree:
        rootNode = new QuadTreeNode(-2048.0f, 2048.0f, -2048.0f, 2048.0f, this);
        break;
    default:
        assert(false);
        break;
    }
}

SceneGraph::~SceneGraph()
{
    if (rootNode)
        rootNode->release();
}

void SceneGraph::DrawAll(double dt) const
{
    rootNode->Update(dt);

    RenderQueue rq(ambientColor);

    if (activeCamera)
        activeCamera->Update();
    if (activeCamera)
        rq.SetCameraNode(activeCamera);
    if (activeSky)
        rq.SetSkyNode(activeSky);

    for (auto iter : directionalLights)
        iter->Draw(rq);

    rootNode->Draw(rq);

    GetRenderer()->DrawAll(rq, dt);
}

MeshSceneNode * SceneGraph::AddMeshNode(const std::wstring & meshFileName, const Material * material)
{
    auto pCtxMgr = GetContextManager();
    IMeshDataPtr data = pCtxMgr->LoadMesh(meshFileName);
    MeshSceneNode * out = new MeshSceneNode(data, material, this);
    rootNode->AddChild(out);
    return out;
}

MeshSceneNode * SceneGraph::AddMeshNode(IMeshDataPtr dataPtr, const Material * mat)
{
    MeshSceneNode * out = new MeshSceneNode(dataPtr, mat, this);
    rootNode->AddChild(out);
    return out;
}

SpriteSceneNode * SceneGraph::AddSpriteNode(const std::wstring & textureName,
                                            int spriteLayer/* = 0*/,
                                            const MathLib::vec2f & leftTopTextureCoords/* = {0.0f, 0.0f}*/,
                                            const MathLib::vec2f & rightBottomTextureCoords/* = {1.0f, 1.0f}*/)
{
    CreateSpriteRequisites();

    ITexturePtr texture = GetContextManager()->LoadTexture(textureName);
    SpriteSceneNode * out = new SpriteSceneNode(texture, leftTopTextureCoords, rightBottomTextureCoords, this, spriteLayer);
    rootNode->AddChild(out);
    return out;
}

AnimatedSpriteSceneNode * SceneGraph::AddAnimatedSpriteNode(const std::wstring & textureName,
                                                            int spriteLayer/* = 0*/)
{
    CreateSpriteRequisites();

    ITexturePtr texture = GetContextManager()->LoadTexture(textureName);
    AnimatedSpriteSceneNode * out = new AnimatedSpriteSceneNode(texture, this, spriteLayer);
    rootNode->AddChild(out);
    return out;
}

EmptySceneNode * ShiftEngine::SceneGraph::AddEmptyNode()
{
    EmptySceneNode * out = new EmptySceneNode(this);
    rootNode->AddChild(out);
    return out;
}

void SceneGraph::CreateSpriteRequisites()
{
    if (!spriteMesh)
    {
        // TODO: this may take place in mesh manager or somewhere not in scene graph for future
        // this is asset/mesh, and this is not zone of responsibility of scene graph
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

    if (!spriteProgram)
    {
        spriteProgram = GetContextManager()->LoadShader(L"SpriteShader.fx");
    }
}

CameraSceneNode * SceneGraph::AddCameraSceneNode(CameraViewType cameraType)
{
    auto engineSettings = GetContextManager()->GetEngineSettings();

    CameraSceneNode * cam = new CameraSceneNode((float)engineSettings.screenWidth,
        (float)engineSettings.screenHeight,
                                                engineSettings.zNear,
                                                engineSettings.zFar,
                                                60.0f,
                                                cameraType,
                                                this);
    rootNode->AddChild(cam);

    if (!activeCamera)
        activeCamera = cam;

    return cam;
}

void SceneGraph::SetActiveCamera(CameraSceneNode * camera)
{
    this->activeCamera = camera;
}

CameraSceneNode * SceneGraph::GetActiveCamera() const
{
    return activeCamera;
}

SkySceneNode * SceneGraph::AddSkySceneNode()
{
    Material material(GetContextManager()->LoadShader(L"sky.fx"));
    IMeshDataPtr mesh = GetContextManager()->LoadMesh(L"sky.lim");
    SkySceneNode * out = new SkySceneNode(&material, mesh, this);
    activeSky = out;
    return out;
}

LightSceneNode * SceneGraph::AddDirectionalLightNode(const MathLib::vec3f & direction, const MathLib::vec3f & color)
{
    LightSceneNode * out = new LightSceneNode(this, LNT_Directional, color);
    out->SetDirection(direction);
    out->addRef();
    directionalLights.push_back(out);
    return out;
}

LightSceneNode * SceneGraph::AddPointLightNode(const MathLib::vec3f & pos, float radius, const MathLib::vec3f & color)
{
    LightSceneNode * out = new LightSceneNode(this, LNT_Point, color);
    out->SetRadius(radius);
    out->SetLocalPosition(pos);
    rootNode->AddChild(out);
    return out;
}

void SceneGraph::SetAmbientColor(const MathLib::vec3f & color)
{
    ambientColor = color;
}

MathLib::vec3f SceneGraph::GetAmbientColor() const
{
    return ambientColor;
}

SkySceneNode * SceneGraph::GetActiveSkyNode() const
{
    return activeSky;
}

void SceneGraph::RemoveDirectionalLightNode(LightSceneNode * node)
{
    for (auto iter = directionalLights.begin(); iter != directionalLights.end(); ++iter)
    {
        if (*iter != node)
            continue;

        (*iter)->release();
        iter = directionalLights.erase(iter);
        if (iter == directionalLights.end())
            return;
    }
}

void SceneGraph::MoveNodeCallback(ISceneNode * node)
{
    switch (type)
    {
    case SGT_Plain:
        return;
    case SGT_QuadTree:
    {
        node->addRef();
        node->RemoveParent();
        rootNode->AddChild(node);
        node->release();
    }
    break;
    default:
        assert(false);
    }
}
