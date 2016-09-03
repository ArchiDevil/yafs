#pragma once

#include "ISceneNode.h"
#include "PlainTreeNode.h"
#include "QuadTreeNode.h"
#include "CameraSceneNode.h"
#include "MeshSceneNode.h"
#include "SkySceneNode.h"
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

enum class CameraViewType;

class ISceneNode;
class PlainTreeNode;
class QuadTreeNode;
class MeshSceneNode;
class CameraSceneNode;
class SkySceneNode;
class LightSceneNode;
class SpriteSceneNode;
class AnimatedSpriteSceneNode;
class EmptySceneNode;

class SceneGraph final
{
public:
    SceneGraph(SceneGraphType graphType = SGT_Plain);
    ~SceneGraph();

    // MESHES
    MeshSceneNode * AddMeshNode(const std::wstring & meshFileName, const Material * mat); //tries to load mesh with meshLoader
    MeshSceneNode * AddMeshNode(IMeshDataPtr dataPtr, const Material * mat);
    SpriteSceneNode * AddSpriteNode(const std::wstring & textureName,
                                    int spriteLayer = 0,
                                    const MathLib::vec2f & leftTopTextureCoords = {0.0f, 0.0f},
                                    const MathLib::vec2f & rightBottomTextureCoords = {1.0f, 1.0f});
    AnimatedSpriteSceneNode * AddAnimatedSpriteNode(const std::wstring & textureName,
                                                    int spriteLayer = 0);
    EmptySceneNode * AddEmptyNode();

    // LIGHTS
    LightSceneNode * AddDirectionalLightNode(const MathLib::vec3f & direction,
                                             const MathLib::vec3f & color = {1.0f, 1.0f, 1.0f});
    void RemoveDirectionalLightNode(LightSceneNode * node);
    LightSceneNode * AddPointLightNode(const MathLib::vec3f & pos,
                                       float radius,
                                       const MathLib::vec3f & color = {1.0f, 1.0f, 1.0f});
    void SetAmbientColor(const MathLib::vec3f & color);
    MathLib::vec3f GetAmbientColor() const;

    // SKY
    SkySceneNode * AddSkySceneNode();
    SkySceneNode * GetActiveSkyNode() const;

    // CAMERA
    CameraSceneNode * AddCameraSceneNode(CameraViewType cameraType);
    void SetActiveCamera(CameraSceneNode * camera);
    CameraSceneNode * GetActiveCamera() const;

    // OTHER
    void DrawAll(double dt) const;
    void MoveNodeCallback(ISceneNode * node);

private:
    void CreateSpriteRequisites();

    ISceneNode *                    rootNode = nullptr;
    CameraSceneNode *               activeCamera = nullptr;
    SkySceneNode *                  activeSky = nullptr;
    VertexSemantic                  skySemantic;
    std::vector<LightSceneNode*>    directionalLights;
    SceneGraphType                  type = SGT_Plain;
    MathLib::vec3f                  ambientColor = {};

    IProgramPtr                     spriteProgram = nullptr;
    IMeshDataPtr                    spriteMesh = nullptr;

};

}
