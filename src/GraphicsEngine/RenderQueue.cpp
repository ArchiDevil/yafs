#include "RenderQueue.h"

#include "SceneGraph/MeshSceneNode.h"
#include "SceneGraph/CameraSceneNode.h"
#include "SceneGraph/SkySceneNode.h"
#include "SceneGraph/LightSceneNode.h"

using namespace ShiftEngine;

RenderQueue::RenderQueue(const MathLib::vec3f & _ambientColor)
    : ambientColor(_ambientColor)
{}

RenderQueue::~RenderQueue()
{}

void RenderQueue::AddRenderableNode(MeshSceneNode * node)
{
    if (!node)
        return;

    meshes.push_back(node);
}

RenderVector & RenderQueue::GetRenderableNodes()
{
    return meshes;
}

SpritesVault & RenderQueue::GetSpriteNodes()
{
    return sprites;
}

void RenderQueue::SetCameraNode(CameraSceneNode * node)
{
    activeCamera = node;
}

CameraSceneNode * RenderQueue::GetActiveCamera() const
{
    return activeCamera;
}

void RenderQueue::SetSkyNode(SkySceneNode * node)
{
    activeSky = node;
}

SkySceneNode * RenderQueue::GetActiveSky() const
{
    return activeSky;
}

void RenderQueue::AddLightNode(LightSceneNode * node)
{
    if (!node)
        return;

    lights.push_back(node);
}

void RenderQueue::AddSpriteNode(SpriteSceneNode * node, int renderingLayer)
{
    if (!node)
        return;

    sprites[renderingLayer].push_back(node);
}

const LightsVector & RenderQueue::GetLights() const
{
    return lights;
}

MathLib::vec3f RenderQueue::GetAmbientColor() const
{
    return ambientColor;
}
