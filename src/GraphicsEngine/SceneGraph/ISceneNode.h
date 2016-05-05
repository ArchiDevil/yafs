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

    ISceneNode();
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

    MathLib::vec3f GetPosition() const;
    void SetPosition(const MathLib::vec3f & val);

    MathLib::vec3f GetScale() const;
    void SetScale(const MathLib::vec3f & val);
    void SetScale(float val);

    MathLib::qaFloat GetRotation() const;
    void SetRotation(const MathLib::qaFloat & val);
    void RotateBy(const MathLib::qaFloat & val);

    void SetSceneGraph(SceneGraph * val);

    virtual MathLib::mat4f GetWorldMatrix() const;

    virtual MathLib::AABB GetBBox() const = 0;

protected:
    virtual void PushToRQ(RenderQueue & rq) = 0;
    virtual CameraFrustum::CullingStatus CheckVisibility(const CameraSceneNode & activeCam) const;

    void CreateWorldMatrix();

    SceneGraph * pSceneGraph = nullptr;;

private:
    MathLib::vec3f Position = {0.0f, 0.0f, 0.0f};
    MathLib::vec3f Scale = {1.0f, 1.0f, 1.0f};
    MathLib::qaFloat Rotation = MathLib::quaternionFromVecAngle(MathLib::vec3f(0.0f, 0.0f, 1.0f), 0.0f);

    MathLib::mat4f worldMatrix;

    ISceneNode * parent = nullptr;
    ChildsList children;    //semi-automatic shared ptrs
};
}
