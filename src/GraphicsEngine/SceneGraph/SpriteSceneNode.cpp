#include "SpriteSceneNode.h"

using namespace ShiftEngine;

SpriteSceneNode::SpriteSceneNode(const IMeshDataPtr & data, const Material * material)
    : MeshNode(data, material)
{
}

void SpriteSceneNode::SetDataPtr(IMeshDataPtr data)
{
    // unable to change mesh data for sprites
}

void SpriteSceneNode::SetMaterial(const Material * val)
{
    // unable to change material for sprites
}

