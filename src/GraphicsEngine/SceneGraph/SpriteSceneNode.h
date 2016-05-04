#pragma once

#include "ISceneNode.h"
#include "../IMeshData.h"
#include "../ITexture.h"

namespace ShiftEngine
{

class SpriteSceneNode final : public ISceneNode
{
public:
    SpriteSceneNode(const ITexturePtr & texture, SceneGraph * sceneGraph);
    MathLib::AABB GetBBox() const override;

    MathLib::vec4f GetMaskColor() const;
    void SetMaskColor(const MathLib::vec4f & color);
    const ITexturePtr & GetTexture() const;

protected:
    void PushToRQ(RenderQueue & rq) override;

    MathLib::vec4f maskColor = {1.0f, 1.0f, 1.0f, 1.0f};
    ITexturePtr texture = nullptr;

};

}
