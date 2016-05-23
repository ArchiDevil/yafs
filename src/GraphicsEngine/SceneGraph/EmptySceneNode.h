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
    void PushToRQ(RenderQueue & rq) override
    {
    }

    CameraFrustum::CullingStatus CheckVisibility(const CameraSceneNode & activeCam) const override
    {
        return CameraFrustum::CullingStatus::CS_In;
    }

    // Унаследовано через ISceneNode
    MathLib::AABB GetBBox() const override
    {
        return {};
    }
};

}
