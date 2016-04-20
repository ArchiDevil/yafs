#pragma once

#include "ISceneNode.h"
#include "PlainTreeNode.h"
#include "QuadTreeNode.h"
#include "CameraSceneNode.h"
#include "MeshNode.h"
#include "SkySceneNode.h"

namespace ShiftEngine
{
    enum SceneGraphType
    {
        SGT_Plain,
        SGT_QuadTree,
        //SGT_OctTree
    };

    class ISceneNode;
    class PlainTreeNode;
    class QuadTreeNode;
    class MeshNode;
    class CameraSceneNode;
    class SkySceneNode;
    class LightNode;

    class SceneGraph
    {
    public:
        SceneGraph(SceneGraphType graphType = SGT_Plain);
        ~SceneGraph();

        // MESHES
        MeshNode * AddMeshNode(const std::wstring & meshFileName, const Material * mat); //tries to load mesh with meshLoader
        MeshNode * AddMeshNode(IMeshDataPtr dataPtr, const Material * mat);

        // LIGHTS
        LightNode * AddDirectionalLightNode(const MathLib::vec3f & direction, const MathLib::vec3f & color = MathLib::vec3f(1.0f, 1.0f, 1.0f));
        void RemoveDirectionalLightNode(LightNode * node);
        LightNode * AddPointLightNode(const MathLib::vec3f & pos, float radius, const MathLib::vec3f & color = MathLib::vec3f(1.0f, 1.0f, 1.0f));
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
        void MoveNodeCallback(ISceneNode * node);
        void DrawAll(double dt) const;

    protected:
        ISceneNode * rootNode = nullptr;
        CameraSceneNode * activeCamera = nullptr;
        SkySceneNode * activeSky = nullptr;
        VertexSemantic skySemantic;
        std::vector<LightNode*> directionalLights;
        SceneGraphType type = SGT_Plain;
        MathLib::vec3f ambientColor = {};

    };
}
