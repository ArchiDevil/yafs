#include "LightSceneNode.h"

#include "../RenderQueue.h"
#include "SceneGraph.h"

using namespace ShiftEngine;
using namespace MathLib;

LightSceneNode::LightSceneNode(SceneGraph * sceneGraph, LightNodeType _type, const vec3f & _color)
    : ISceneNode(sceneGraph)
    , lightColor(_color)
    , type(_type)
{
}

void LightSceneNode::PushToRQ(RenderQueue & rq)
{
    if (isActive)
        rq.AddLightNode(this);
}

void LightSceneNode::SetColor(const vec3f & color)
{
    this->lightColor = color;
}

vec3f LightSceneNode::GetColor() const
{
    return lightColor;
}

LightNodeType LightSceneNode::GetType() const
{
    return type;
}

float LightSceneNode::GetRadius() const
{
    return radius;
}

void LightSceneNode::SetRadius(float val)
{
    radius = val;
}

vec3f LightSceneNode::GetDirection() const
{
    return direction;
}

void LightSceneNode::SetDirection(const vec3f & val)
{
    direction = val;
}

void LightSceneNode::SetActive(bool active)
{
    this->isActive = active;
}

bool LightSceneNode::IsActive() const
{
    return isActive;
}

void LightSceneNode::KillSelf()
{
    if (ISceneNode::pSceneGraph)
    {
        if (type == LNT_Directional)
            ISceneNode::pSceneGraph->RemoveDirectionalLightNode(this);
        else
            ISceneNode::KillSelf();
    }
}

AABB LightSceneNode::GetBBox() const
{
    return {};
}
