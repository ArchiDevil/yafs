#pragma once

#include "ISceneNode.h"
#include "../ITexture.h"

namespace ShiftEngine
{

class SpriteSceneNode : public ISceneNode
{
public:
    enum class SpriteDrawingMode
    {
        Default,
        Additive
    };

    SpriteSceneNode(const ITexturePtr & texture,
                    SceneGraph * sceneGraph,
                    int spriteLayer);

    SpriteSceneNode(const ITexturePtr & texture,
                    const MathLib::vec2f& leftTopCoords,
                    const MathLib::vec2f& rightBottomCoords,
                    SceneGraph * sceneGraph,
                    int spriteLayer);

    void                        SetMaskColor(const MathLib::vec4f & color);
    void                        SetDrawingMode(SpriteDrawingMode mode);

    MathLib::AABB               GetBBox() const override;
    MathLib::vec4f              GetMaskColor() const;
    SpriteDrawingMode           GetDrawingMode() const;
    const ITexturePtr &         GetTexture() const;
    MathLib::matrix<float, 3>   GetTextureMatrix() const;
    int                         GetRenderingLayer() const;

protected:
    void PushToRQ(RenderQueue & rq) override;
    void CalculateTextureMatrix(const MathLib::vec2f& leftTopCoords, const MathLib::vec2f& rightBottomCoords);

    MathLib::vec4f maskColor = {1.0f, 1.0f, 1.0f, 1.0f};
    SpriteDrawingMode drawingMode = SpriteDrawingMode::Default;
    MathLib::matrix<float, 3> textureMatrix = MathLib::matrixIdentity<float, 3>();
    ITexturePtr texture = nullptr;
    int layer = 0;

};

}
