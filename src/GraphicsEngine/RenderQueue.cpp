#include "RenderQueue.h"

#include "SceneGraph/CameraSceneNode.h"

using namespace ShiftEngine;

RenderQueue::RenderQueue(const MathLib::vec3f & _ambientColor)
    : ambientColor(_ambientColor)
{}

SpritesVault & RenderQueue::GetSpriteNodes()
{
    return sprites;
}

void RenderQueue::SetCameraNode(CameraSceneNode * node)
{
    activeCamera = node;
}

CameraSceneNode& RenderQueue::GetActiveCamera() const
{
    return *activeCamera;
}

void RenderQueue::AddSpriteNode(SpriteSceneNode * node, int renderingLayer)
{
    if (!node)
        return;

    sprites[renderingLayer].push_back(node);
}

MathLib::vec3f RenderQueue::GetAmbientColor() const
{
    return ambientColor;
}
