#pragma once

#include "ISceneNode.h"
#include "PlainTreeNode.h"
#include "QuadTreeNode.h"
#include "CameraSceneNode.h"
#include "SpriteSceneNode.h"
#include "AnimatedSpriteSceneNode.h"
#include "EmptySceneNode.h"

#include <string>
#include <vector>

namespace ShiftEngine
{

enum SceneGraphType
{
    SGT_Plain,
    SGT_QuadTree,
    //SGT_OctTree
};

// TODO Create something more flexible.
enum SpriteLayer
{
    SL_Floor = 0,
    SL_Entities,
};

enum class CameraViewType;

class ISceneNode;
class PlainTreeNode;
class QuadTreeNode;
class CameraSceneNode;
class SpriteSceneNode;
class AnimatedSpriteSceneNode;
class EmptySceneNode;

class SceneGraph final
{
public:
    SceneGraph(SceneGraphType graphType = SGT_Plain);
    ~SceneGraph();

    // MESHES
    SpriteSceneNode * AddSpriteNode(const std::wstring & textureName,
                                    SpriteLayer spriteLayer = SL_Floor,
                                    const MathLib::vec2f & leftTopTextureCoords = {0.0f, 0.0f},
                                    const MathLib::vec2f & rightBottomTextureCoords = {1.0f, 1.0f});
    AnimatedSpriteSceneNode * AddAnimatedSpriteNode(const std::wstring & textureName,
                                                    int spriteLayer = 0);

    // LIGHTS
    void SetAmbientColor(const MathLib::vec3f & color);
    MathLib::vec3f GetAmbientColor() const;

    // CAMERA
    CameraSceneNode * AddCameraSceneNode(CameraViewType cameraType);
    void SetActiveCamera(CameraSceneNode * camera);
    CameraSceneNode * GetActiveCamera() const;

    // OTHER
    void DrawAll(double dt) const;
    void MoveNodeCallback(ISceneNode * node);

private:
    ISceneNode *                    rootNode = nullptr;
    CameraSceneNode *               activeCamera = nullptr;
    SceneGraphType                  type = SGT_Plain;
    MathLib::vec3f                  ambientColor = {};

};

}
