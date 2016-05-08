#include "SpriteSceneNode.h"

#include "../RenderQueue.h"

using namespace ShiftEngine;

SpriteSceneNode::SpriteSceneNode(const ITexturePtr & texture,
                                 SceneGraph * sceneGraph)
    : SpriteSceneNode(texture, {0.0f, 0.0f}, {1.0f, 1.0f}, sceneGraph)
{
}

SpriteSceneNode::SpriteSceneNode(const ITexturePtr & texture,
                                 const MathLib::vec2f& leftTopCoords,
                                 const MathLib::vec2f& rightBottomCoords,
                                 SceneGraph * sceneGraph)
    : ISceneNode(sceneGraph)
    , texture(texture)
{
    float x_scale = rightBottomCoords.x - leftTopCoords.x;
    float y_scale = rightBottomCoords.y - leftTopCoords.y;

    float x_shift = leftTopCoords.x;
    float y_shift = leftTopCoords.y;

    textureMatrix = MathLib::matrixScaling(x_scale, y_scale) * MathLib::matrixTranslation(x_shift, y_shift);
}

void ShiftEngine::SpriteSceneNode::PushToRQ(RenderQueue & rq)
{
    rq.AddSpriteNode(this);
}

MathLib::AABB ShiftEngine::SpriteSceneNode::GetBBox() const
{
    MathLib::mat4f matWorld = GetWorldMatrix();
    MathLib::vec4f points[8];
    MathLib::AABB bbox = {{}, {}};

    points[0] = {bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f};
    points[1] = {bbox.bMin.x, bbox.bMin.y, bbox.bMax.z, 1.0f};
    points[2] = {bbox.bMin.x, bbox.bMax.y, bbox.bMin.z, 1.0f};
    points[3] = {bbox.bMax.x, bbox.bMin.y, bbox.bMin.z, 1.0f};
    points[4] = {bbox.bMax.x, bbox.bMax.y, bbox.bMin.z, 1.0f};
    points[5] = {bbox.bMin.x, bbox.bMax.y, bbox.bMax.z, 1.0f};
    points[6] = {bbox.bMax.x, bbox.bMin.y, bbox.bMax.z, 1.0f};
    points[7] = {bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f};

    MathLib::vec3f min, max;

    for (int i = 0; i < 8; i++)
        points[i] = MathLib::vec4Transform(points[i], matWorld);

    min.x = points[0].x;
    min.y = points[0].y;
    min.z = points[0].z;
    max.x = points[0].x;
    max.y = points[0].y;
    max.z = points[0].z;
    for (int i = 0; i < 8; i++)
    {
        if (points[i].x < min.x)
            min.x = points[i].x;
        if (points[i].y < min.y)
            min.y = points[i].y;
        if (points[i].z < min.z)
            min.z = points[i].z;

        if (points[i].x > max.x)
            max.x = points[i].x;
        if (points[i].y > max.y)
            max.y = points[i].y;
        if (points[i].z > max.z)
            max.z = points[i].z;
    }

    return {{min.x, min.y, min.z}, {max.x, max.y, max.z}};
}

MathLib::vec4f ShiftEngine::SpriteSceneNode::GetMaskColor() const
{
    return maskColor;
}

const ITexturePtr & ShiftEngine::SpriteSceneNode::GetTexture() const
{
    return texture;
}

MathLib::matrix<float, 3> ShiftEngine::SpriteSceneNode::GetTextureMatrix() const
{
    return textureMatrix;
}

void ShiftEngine::SpriteSceneNode::SetMaskColor(const MathLib::vec4f & color)
{
    maskColor = color;
}
