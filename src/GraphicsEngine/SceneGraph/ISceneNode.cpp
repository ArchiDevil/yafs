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

MathLib::mat4f ISceneNode::GetWorldMatrix() const
{
    return worldMatrix;
}

MathLib::mat4f ISceneNode::GetLocalMatrix() const
{
    return localMatrix;
}

void ISceneNode::CreateMatrices()
{
    MathLib::matrix<float, 4> scale = MathLib::matrixScaling(Scale);
    MathLib::matrix<float, 4> rotation = Rotation.to_matrix();
    MathLib::matrix<float, 4> position = MathLib::matrixTranslation(Position);

    if (parent)
        worldMatrix = localMatrix * parent->GetWorldMatrix();
    else
        worldMatrix = localMatrix;

    localMatrix = scale * rotation * position;
}

MathLib::vec3f ISceneNode::GetWorldPosition() const
{
    if (parent)
        return parent->GetWorldPosition() + Position;
    return Position;
}

MathLib::vec3f ISceneNode::GetLocalPosition() const
{
    return Position;
}

void ISceneNode::SetLocalPosition(const MathLib::vec3f & val)
{
    Position = val;
    CreateMatrices();

    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

MathLib::vec3f ISceneNode::GetWorldScale() const
{
    if (parent)
        return parent->Scale + Scale;

    return Scale;
}

MathLib::vec3f ISceneNode::GetLocalScale() const
{
    return Scale;
}

void ISceneNode::SetLocalScale(const MathLib::vec3f & val)
{
    Scale = val;
    CreateMatrices();
    if (pSceneGraph)
        pSceneGraph->MoveNodeCallback(this);
}

void ISceneNode::SetLocalScale(float val)
{
    SetLocalScale(MathLib::vec3f(val, val, val));
}

MathLib::qaFloat ISceneNode::GetWorldRotation() const
{
    if (parent)
        return Rotation * parent->GetWorldRotation();
    return Rotation;
}

MathLib::qaFloat ISceneNode::GetLocalRotation() const
{
    return Rotation;
}

void ISceneNode::SetLocalRotation(const MathLib::qaFloat & val)
{
    Rotation = val;
}

void ISceneNode::RotateByLocal(const MathLib::qaFloat & val)
{
    Rotation = Rotation * val;
}

CameraFrustum::CullingStatus ISceneNode::CheckVisibility(const CameraSceneNode & activeCam) const
{
    MathLib::mat4f matWorld = GetWorldMatrix();
    MathLib::AABB bbox = GetBBox();

    MathLib::vec4f vecMin = {bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f};
    MathLib::vec4f vecMax = {bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f};
    vecMin = MathLib::vec4Transform(vecMin, matWorld);
    vecMax = MathLib::vec4Transform(vecMax, matWorld);
    MathLib::AABB newBbox(MathLib::vec3f(vecMin.x, vecMin.y, vecMin.z), MathLib::vec3f(vecMax.x, vecMax.y, vecMax.z));

    return activeCam.GetFrustumPtr()->CheckAABB(newBbox);
}

SceneGraph * ISceneNode::GetSceneGraph() const
{
    return pSceneGraph;
}
