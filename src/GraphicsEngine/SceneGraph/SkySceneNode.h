#pragma once

#include "MeshSceneNode.h"

namespace ShiftEngine
{

class SkySceneNode final : public MeshSceneNode
{
public:
    SkySceneNode(const Material * material, const IMeshDataPtr & meshData, SceneGraph * sceneGraph);

    virtual void PushToRQ(RenderQueue & rq);
    virtual int CheckVisibility(CameraSceneNode * activeCam) const;

};

}
