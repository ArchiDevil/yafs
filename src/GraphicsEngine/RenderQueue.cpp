#include "RenderQueue.h"

#include "SceneGraph/MeshNode.h"
#include "SceneGraph/CameraSceneNode.h"
#include "SceneGraph/SkySceneNode.h"
#include "SceneGraph/LightNode.h"

ShiftEngine::RenderQueue::RenderQueue(const MathLib::vec3f & _ambientColor)
    : ambientColor(_ambientColor)
{
}

ShiftEngine::RenderQueue::~RenderQueue()
{
}

void ShiftEngine::RenderQueue::AddRenderableNode(MeshNode * node)
{
    if (!node)
        return;

    meshNodesVector.push_back(node);
}

ShiftEngine::RenderVector & ShiftEngine::RenderQueue::GetRenderableNodes()
{
    return meshNodesVector;
}

void ShiftEngine::RenderQueue::SetCameraNode(CameraSceneNode * node)
{
    activeCamera = node;
}

ShiftEngine::CameraSceneNode * ShiftEngine::RenderQueue::GetActiveCamera() const
{
    return activeCamera;
}

void ShiftEngine::RenderQueue::SetSkyNode(SkySceneNode * node)
{
    activeSky = node;
}

ShiftEngine::SkySceneNode * ShiftEngine::RenderQueue::GetActiveSky() const
{
    return activeSky;
}

void ShiftEngine::RenderQueue::AddLightNode(LightNode * node)
{
    lights.push_back(node);
}

const ShiftEngine::LightsVector & ShiftEngine::RenderQueue::GetLights() const
{
    return lights;
}

MathLib::vec3f ShiftEngine::RenderQueue::GetAmbientColor() const
{
    return ambientColor;
}
