#include "../ShiftEngine.h"

#include "SkySceneNode.h"
#include "MeshNode.h"

using namespace ShiftEngine;

SkySceneNode::SkySceneNode(const Material * material, const IMeshDataPtr & meshData)
    : MeshNode(meshData, material)
{}

void SkySceneNode::PushToRQ(RenderQueue & rq)
{
    //auto pos = GetSceneGraph()->GetActiveCamera()->GetPosition();
    //this->SetPosition(Vector3F(pos.x, pos.y, pos.z));
    rq.SetSkyNode(this);
}

CameraFrustum::CullingStatus SkySceneNode::CheckVisibility(const CameraSceneNode & activeCam) const
{
    return CameraFrustum::CullingStatus::CS_In;
}

void ShiftEngine::SkySceneNode::SetMaterial(const ShiftEngine::Material * val)
{}

void ShiftEngine::SkySceneNode::SetDataPtr(IMeshDataPtr data)
{}
