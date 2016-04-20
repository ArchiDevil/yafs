#include "ISceneNode.h"

#include "CameraSceneNode.h"
#include "SceneGraph.h"

#include <algorithm>

using namespace ShiftEngine;
using namespace MathLib;

ISceneNode::ISceneNode()
{
    CreateWorldMatrix();
}

ISceneNode::~ISceneNode()
{
    for (auto child : children)
        child->release();
}

void ISceneNode::AddChild(ISceneNode * node)
{
    //TODO: fix memleak here
    if (!node)
        return;

    //every node must have only 1 parent

    node->addRef(); //to avoid killing by parent

    if (node->parent)
        node->parent->RemoveChild(node);

    node->parent = this;
    //find empty place
    for (auto & elem : children)
    {
        if (!elem)
        {
            elem = node;
            return;
        }
    }

    children.push_back(node);
}

void ISceneNode::RemoveChild(ISceneNode * node)
{
    node->release();
    auto iter = std::find(children.begin(), children.end(), node);
    if (iter != children.end())
    {
        *iter = nullptr;
        *iter = *(--children.end());
        children.pop_back();
    }
}

void ISceneNode::SetParent(ISceneNode * _parent)
{
    if (_parent)
        _parent->AddChild(this);    //it will set parent to node
    else
        parent = _parent;
}

void ISceneNode::RemoveParent()
{
    if (this->parent)
        parent->RemoveChild(this);
    parent = nullptr;
}

void ISceneNode::KillSelf()
{
    if (this->parent)
        parent->RemoveChild(this);
}

ISceneNode * ISceneNode::GetParent() const
{
    return parent;
}

const ISceneNode::ChildsList & ISceneNode::GetChilds() const
{
    return children;
}

void ISceneNode::Draw(RenderQueue & rq)
{
    this->PushToRQ(rq);

    if (!children.empty())
        for (auto child : children)
            child->Draw(rq);
}

mat4f ISceneNode::GetWorldMatrix() const
{
    if (parent)
        return worldMatrix * parent->GetWorldMatrix();
    else
        return worldMatrix;
}

void ISceneNode::CreateWorldMatrix()
{
    matrix<float, 4> scale;
    matrix<float, 4> rotation;
    matrix<float, 4> position;

    scale = matrixScaling(Scale);
    rotation = Rotation.to_matrix();
    position = matrixTranslation(Position);

    worldMatrix = scale * rotation * position;
}

vec3f ISceneNode::GetPosition() const
{
    if (parent)
        return parent->GetPosition() + Position;

    return Position;
}

void ISceneNode::SetPosition(const vec3f & val)
{
    Position = val;
    CreateWorldMatrix();

    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

vec3f ISceneNode::GetScale() const
{
    if (parent)
        return parent->Scale + Scale;

    return Scale;
}

void ISceneNode::SetScale(const vec3f & val)
{
    Scale = val;
    CreateWorldMatrix();

    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

void ISceneNode::SetScale(float val)
{
    SetScale({val, val, val});
}

qaFloat ISceneNode::GetRotation() const
{
    return Rotation;
}

void ISceneNode::SetRotation(const qaFloat & val)
{
    Rotation = val;
}

void ISceneNode::RotateBy(const qaFloat & val)
{
    Rotation = Rotation * val;
}

CameraFrustum::CullingStatus ShiftEngine::ISceneNode::CheckVisibility(const CameraSceneNode & activeCam) const
{
    mat4f matWorld = GetWorldMatrix();
    AABB bbox = GetBBox();

    vec4f vecMin = { bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f };
    vec4f vecMax = { bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f };
    vecMin = vec4Transform(vecMin, matWorld);
    vecMax = vec4Transform(vecMax, matWorld);
    AABB newBbox({vecMin.x, vecMin.y, vecMin.z}, {vecMax.x, vecMax.y, vecMax.z});

    return activeCam.GetFrustumPtr()->CheckAABB(newBbox);
}

void ISceneNode::SetSceneGraph(SceneGraph * val)
{
    pSceneGraph = val;
}
