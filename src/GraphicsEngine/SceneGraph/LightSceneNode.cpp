#include "LightSceneNode.h"

#include "../RenderQueue.h"
#include "SceneGraph.h"

using namespace ShiftEngine;
using namespace MathLib;

LightSceneNode::LightSceneNode(SceneGraph * sceneGraph, LightNodeType _type, const vec3f & _color)
    : ISceneNode(sceneGraph)
    , color(_color)
    , type(_type)
{
}

void LightSceneNode::PushToRQ(RenderQueue & rq)
{
    if (active)
        rq.AddLightNode(this);
}

void LightSceneNode::SetColor(const vec3f & color)
{
    this->color = color;
}

vec3f LightSceneNode::GetColor() const
{
    return color;
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
    this->active = active;
}

bool LightSceneNode::IsActive() const
{
    return active;
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
