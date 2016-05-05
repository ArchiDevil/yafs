#pragma once

#include "CameraFrustum.h"

#include <Utilities/refcounted.h>
#include <MathLib/math.h>

#include <vector>

namespace ShiftEngine
{

class SceneGraph;
class RenderQueue;
class CameraSceneNode;

class ISceneNode : public Refcounted
{
public:
    typedef std::vector<ISceneNode*> ChildsList;

    ISceneNode(SceneGraph * sceneGraph);
    virtual ~ISceneNode();

    ISceneNode(const ISceneNode&) = delete;
    ISceneNode(ISceneNode&&) = delete;
    ISceneNode& operator=(const ISceneNode&) = delete;
    ISceneNode& operator=(ISceneNode&&) = delete;

    void Draw(RenderQueue & rq);

    virtual void AddChild(ISceneNode * node);
    void RemoveChild(ISceneNode * node);
    const ChildsList & GetChilds() const;
    virtual void KillSelf();

    ISceneNode * GetParent() const;
    void SetParent(ISceneNode * _parent);
    void RemoveParent();

    MathLib::vec3f GetWorldPosition() const;
    MathLib::vec3f GetLocalPosition() const;
    virtual void SetLocalPosition(const MathLib::vec3f & val);

    MathLib::vec3f GetWorldScale() const;
    MathLib::vec3f GetLocalScale() const;
    virtual void SetLocalScale(const MathLib::vec3f & val);
    virtual void SetLocalScale(float val);

    MathLib::qaFloat GetWorldRotation() const;
    MathLib::qaFloat GetLocalRotation() const;
    virtual void SetLocalRotation(const MathLib::qaFloat & val);
    virtual void RotateByLocalQuaternion(const MathLib::qaFloat & val);

    SceneGraph * GetSceneGraph() const;
    virtual MathLib::mat4f GetWorldMatrix() const;
    virtual MathLib::mat4f GetLocalMatrix() const;
    virtual MathLib::AABB GetBBox() const = 0;

protected:
    virtual void PushToRQ(RenderQueue & rq) = 0;
    virtual CameraFrustum::CullingStatus CheckVisibility(const CameraSceneNode & activeCam) const;
    void CreateMatrices();

    SceneGraph * const      pSceneGraph = nullptr;
    ISceneNode *            parent = nullptr;
    ChildsList              children;    //semi-automatic shared ptrs

    MathLib::vec3f          position = {0.0f, 0.0f, 0.0f};
    MathLib::vec3f          scale = {1.0f, 1.0f, 1.0f};
    MathLib::qaFloat        rotation = MathLib::quaternionFromVecAngle(MathLib::vec3f(0.0f, 1.0f, 0.0f), 0.0f);

    MathLib::mat4f          localMatrix = {};
    MathLib::mat4f          worldMatrix = {};
};

}
