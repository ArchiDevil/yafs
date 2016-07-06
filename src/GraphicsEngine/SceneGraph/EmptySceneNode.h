#pragma once

#include "ISceneNode.h"

namespace ShiftEngine
{

// this empty class is intended to handling scene hierarchies`
class EmptySceneNode final : public ISceneNode
{
public:
    EmptySceneNode(SceneGraph * sceneGraph)
        : ISceneNode(sceneGraph)
    {
    }

    ~EmptySceneNode() = default;

protected:
    void PushToRQ(RenderQueue&) override
    {
    }

    CameraFrustum::CullingStatus CheckVisibility(const CameraSceneNode&) const override
    {
        return CameraFrustum::CullingStatus::CS_In;
    }

    MathLib::AABB GetBBox() const override
    {
        return {};
    }
};

}
