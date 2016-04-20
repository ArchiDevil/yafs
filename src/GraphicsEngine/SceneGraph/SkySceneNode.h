#pragma once

#include "../Material.h"

#include <MathLib/math.h>

namespace ShiftEngine
{
    class ISceneNode;
    class MeshNode;
    class CameraSceneNode;

    class SkySceneNode final : public MeshNode
    {
    public:
        SkySceneNode(const Material * material, const IMeshDataPtr & meshData);

        virtual void PushToRQ(RenderQueue & rq);
        virtual CameraFrustum::CullingStatus CheckVisibility(const CameraSceneNode & activeCam) const;
    };
}
