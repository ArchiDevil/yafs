#include "ISceneNode.h"

#include "CameraSceneNode.h"
#include "SceneGraph.h"

#include <algorithm>

using namespace ShiftEngine;
using namespace MathLib;

ISceneNode::ISceneNode(SceneGraph * sceneGraph)
    : pSceneGraph(sceneGraph)
{
    CreateMatrices();
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
    return worldMatrix;
}

mat4f ISceneNode::GetLocalMatrix() const
{
    return localMatrix;
}

void ISceneNode::CreateMatrices()
{
    matrix<float, 4> _scale = matrixScaling(scale);
    matrix<float, 4> _rotation = rotation.to_matrix();
    matrix<float, 4> _position = matrixTranslation(position);

    localMatrix = _scale * _rotation * _position;

    if (parent)
        worldMatrix = localMatrix * parent->GetWorldMatrix();
    else
        worldMatrix = localMatrix;

    MatrixNotifyChilds();
}

vec3f ISceneNode::GetWorldPosition() const
{
    if (parent)
        return parent->GetWorldPosition() + position;
    return position;
}

vec3f ISceneNode::GetLocalPosition() const
{
    return position;
}

void ISceneNode::SetLocalPosition(const vec3f & val)
{
    position = val;
    CreateMatrices();

    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

vec3f ISceneNode::GetWorldScale() const
{
    if (parent)
        return {parent->scale.x * scale.x, parent->scale.y * scale.y, parent->scale.z * scale.z};

    return scale;
}

vec3f ISceneNode::GetLocalScale() const
{
    return scale;
}

void ISceneNode::SetLocalScale(const vec3f & val)
{
    scale = val;
    CreateMatrices();
    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

void ISceneNode::SetLocalScale(float val)
{
    SetLocalScale({val, val, val});
}

qaFloat ISceneNode::GetWorldRotation() const
{
    if (parent)
        return rotation * parent->GetWorldRotation();
    return rotation;
}

qaFloat ISceneNode::GetLocalRotation() const
{
    return rotation;
}

void ISceneNode::SetLocalRotation(const qaFloat & val)
{
    rotation = val;
    CreateMatrices();
}

void ISceneNode::RotateByLocalQuaternion(const qaFloat & val)
{
    rotation = rotation * val;
}

CameraFrustum::CullingStatus ISceneNode::CheckVisibility(const CameraSceneNode & activeCam) const
{
    mat4f matWorld = GetWorldMatrix();
    AABB bbox = GetBBox();

    vec4f vecMin = {bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f};
    vec4f vecMax = {bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f};
    vecMin = vec4Transform(vecMin, matWorld);
    vecMax = vec4Transform(vecMax, matWorld);
    AABB newBbox(vec3f(vecMin.x, vecMin.y, vecMin.z), vec3f(vecMax.x, vecMax.y, vecMax.z));

    return activeCam.GetFrustum().CheckAABB(newBbox);
}

SceneGraph * ISceneNode::GetSceneGraph() const
{
    return pSceneGraph;
}

void ShiftEngine::ISceneNode::Update(double dt)
{
    OnUpdate(dt);
    for (auto& child : children)
    {
        child->Update(dt);
    }
}

void ShiftEngine::ISceneNode::MatrixNotifyChilds()
{
    for (auto& child : children)
    {
        if (child)
        {
            child->OnParentMatrixUpdate();
        }
    }
}

void ShiftEngine::ISceneNode::OnUpdate(double dt)
{
}

void ShiftEngine::ISceneNode::OnParentMatrixUpdate()
{
    CreateMatrices();
}
