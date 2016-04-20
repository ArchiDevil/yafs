#include "LightNode.h"

#include "../RenderQueue.h"
#include "SceneGraph.h"

using namespace ShiftEngine;
using namespace MathLib;

LightNode::LightNode(LightNodeType _type, const vec3f & _color)
    : ISceneNode()
    , color(_color)
    , type(_type)
{
}

void LightNode::PushToRQ(RenderQueue & rq)
{
    if (active)
        rq.AddLightNode(this);
}

void LightNode::SetColor(const vec3f & color)
{
    this->color = color;
}

vec3f LightNode::GetColor() const
{
    return color;
}

LightNodeType LightNode::GetType() const
{
    return type;
}

float LightNode::GetRadius() const
{
    return radius;
}

void LightNode::SetRadius(float val)
{
    radius = val;
}

vec3f LightNode::GetDirection() const
{
    return direction;
}

void LightNode::SetDirection(const vec3f & val)
{
    direction = val;
}

void LightNode::SetActive(bool active)
{
    this->active = active;
}

bool LightNode::IsActive() const
{
    return active;
}

void LightNode::KillSelf()
{
    if (ISceneNode::pSceneGraph)
    {
        if (type == LNT_Directional)
            ISceneNode::pSceneGraph->RemoveDirectionalLightNode(this);
        else
            ISceneNode::KillSelf();
    }
}

AABB LightNode::GetBBox() const
{
    return {};
}
