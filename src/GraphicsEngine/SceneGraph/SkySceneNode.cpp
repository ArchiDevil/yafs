#include "../ShiftEngine.h"

#include "SkySceneNode.h"
#include "MeshSceneNode.h"

using namespace ShiftEngine;

SkySceneNode::SkySceneNode(const Material * material, const IMeshDataPtr & meshData, SceneGraph * sceneGraph)
    : MeshSceneNode(meshData, material, sceneGraph)
{}

void SkySceneNode::PushToRQ(RenderQueue & rq)
{
    //auto pos = GetSceneGraph()->GetActiveCamera()->GetPosition();
    //this->SetPosition(Vector3F(pos.x, pos.y, pos.z));
    rq.SetSkyNode(this);
}

int SkySceneNode::CheckVisibility(CameraSceneNode * /*activeCam*/) const
{
    return CameraFrustum::CS_In;
}
