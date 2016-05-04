#include "LightSceneNode.h"

#include "../RenderQueue.h"
#include "SceneGraph.h"

using namespace ShiftEngine;

LightSceneNode::LightSceneNode(SceneGraph * sceneGraph, LightNodeType _type, const MathLib::vec3f & _color)
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

void LightSceneNode::SetColor(const MathLib::vec3f & color)
{
    this->color = color;
}

MathLib::vec3f LightSceneNode::GetColor() const
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

MathLib::vec3f LightSceneNode::GetDirection() const
{
    return direction;
}

void LightSceneNode::SetDirection(const MathLib::vec3f & val)
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

MathLib::AABB LightSceneNode::GetBBox() const
{
    return {};
}
