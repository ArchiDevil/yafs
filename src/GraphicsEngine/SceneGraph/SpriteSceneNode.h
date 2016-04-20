#pragma once

#include "MeshNode.h"

namespace ShiftEngine
{

class SpriteSceneNode final : public MeshNode
{
public:
    SpriteSceneNode(const IMeshDataPtr & data, const Material * material);

    void SetDataPtr(IMeshDataPtr data) override;
    void SetMaterial(const Material * val) override;

private:

};

}
