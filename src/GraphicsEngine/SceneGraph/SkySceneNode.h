#pragma once

#include "../Material.h"

#include <MathLib/math.h>

namespace ShiftEngine
{
class ISceneNode;
class MeshSceneNode;
class CameraSceneNode;

class SkySceneNode final : public MeshSceneNode
{
public:
    SkySceneNode(const Material * material, const IMeshDataPtr & meshData);

    void PushToRQ(RenderQueue & rq) override;
    CameraFrustum::CullingStatus CheckVisibility(const CameraSceneNode & activeCam) const override;

    // this seems wrong
    void SetMaterial(const ShiftEngine::Material * val) override;
    void SetDataPtr(IMeshDataPtr data) override;
};
}
