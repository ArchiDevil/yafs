#include "SceneGraph.h"

#include "../ShiftEngine.h"
#include "../RenderQueue.h"

#include <cassert>
#include <array>

using namespace ShiftEngine;

SceneGraph::SceneGraph(SceneGraphType graphType /*= SGT_Plain*/)
    : type(graphType)
{
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

    rootNode->Draw(rq);

    GetContextManager()->DrawAll(rq, dt);
}

SpriteSceneNode * SceneGraph::AddSpriteNode(const std::wstring & textureName,
                                            int spriteLayer/* = 0*/,
                                            const MathLib::vec2f & leftTopTextureCoords/* = {0.0f, 0.0f}*/,
                                            const MathLib::vec2f & rightBottomTextureCoords/* = {1.0f, 1.0f}*/)
{
    ITexturePtr texture = GetContextManager()->LoadTexture(textureName);
    SpriteSceneNode * out = new SpriteSceneNode(texture, leftTopTextureCoords, rightBottomTextureCoords, this, spriteLayer);
    rootNode->AddChild(out);
    return out;
}

AnimatedSpriteSceneNode * SceneGraph::AddAnimatedSpriteNode(const std::wstring & textureName,
                                                            int spriteLayer/* = 0*/)
{
    ITexturePtr texture = GetContextManager()->LoadTexture(textureName);
    AnimatedSpriteSceneNode * out = new AnimatedSpriteSceneNode(texture, this, spriteLayer);
    rootNode->AddChild(out);
    return out;
}

CameraSceneNode * SceneGraph::AddCameraSceneNode(CameraViewType cameraType)
{
    auto engineSettings = GetContextManager()->GetEngineSettings();

    CameraSceneNode * cam = new CameraSceneNode((float)engineSettings.screenWidth,
                                                (float)engineSettings.screenHeight,
                                                0.0f,
                                                1.0f,
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

void SceneGraph::SetAmbientColor(const MathLib::vec3f & color)
{
    ambientColor = color;
}

MathLib::vec3f SceneGraph::GetAmbientColor() const
{
    return ambientColor;
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
