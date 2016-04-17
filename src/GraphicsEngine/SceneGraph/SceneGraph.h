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

        void DrawAll(double dt) const;

        MeshNode * AddMeshNode(const std::wstring & meshFileName, const Material * mat);                        //tries to load mesh with meshLoader
        MeshNode * AddMeshNode(IMeshDataPtr dataPtr, const Material * mat);
        CameraSceneNode * AddCameraSceneNode();

        LightNode * AddDirectionalLightNode(const MathLib::Vector3F & direction, const MathLib::Vector3F & color = MathLib::Vector3F(1.0f, 1.0f, 1.0f));
        void RemoveDirectionalLightNode(LightNode * node);

        LightNode * AddPointLightNode(const MathLib::Vector3F & pos, float radius, const MathLib::Vector3F & color = MathLib::Vector3F(1.0f, 1.0f, 1.0f));
        SkySceneNode * AddSkySceneNode();

        SkySceneNode * GetActiveSkyNode() const;

        void SetActiveCamera(CameraSceneNode * camera);
        CameraSceneNode * GetActiveCamera() const;

        void SetAmbientColor(const MathLib::Vector3F & color);
        MathLib::Vector3F GetAmbientColor() const;

        void MoveNodeCallback(ISceneNode * node);

    protected:
        ISceneNode * rootNode = nullptr;
        CameraSceneNode * activeCamera = nullptr;
        SkySceneNode * activeSky = nullptr;
        VertexSemantic skySemantic;
        std::vector<LightNode*> directionalLights;

        SceneGraphType type = SGT_Plain;

        MathLib::Vector3F ambientColor = {};

    };
}
