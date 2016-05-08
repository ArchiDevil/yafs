#pragma once

#include "ISceneNode.h"
#include "../IMeshData.h"
#include "../ITexture.h"

namespace ShiftEngine
{

class SpriteSceneNode : public ISceneNode
{
public:
    SpriteSceneNode(const ITexturePtr & texture,
                    SceneGraph * sceneGraph);
    SpriteSceneNode(const ITexturePtr & texture,
                    const MathLib::vec2f& leftTopCoords,
                    const MathLib::vec2f& rightBottomCoords,
                    SceneGraph * sceneGraph);

    void SetMaskColor(const MathLib::vec4f & color);

    MathLib::AABB GetBBox() const override;
    MathLib::vec4f GetMaskColor() const;
    const ITexturePtr & GetTexture() const;
    MathLib::matrix<float, 3> GetTextureMatrix() const;

protected:
    void PushToRQ(RenderQueue & rq) override;

    MathLib::vec4f maskColor = {1.0f, 1.0f, 1.0f, 1.0f};
    MathLib::matrix<float, 3> textureMatrix = MathLib::matrixIdentity<float, 3>();
    ITexturePtr texture = nullptr;

};

}
