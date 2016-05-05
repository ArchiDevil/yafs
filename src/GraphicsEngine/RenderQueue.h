#pragma once

#include <MathLib/math.h>

#include <vector>

//This class provides storage for scene nodes that will be rendered by renderer.
namespace ShiftEngine
{
    class ISceneNode;
    class MeshSceneNode;
    class CameraSceneNode;
    class SkySceneNode;
    class LightSceneNode;

    typedef std::vector<MeshSceneNode*> RenderVector;
    typedef std::vector<LightSceneNode*> LightsVector;

    class RenderQueue
    {
    public:
        RenderQueue(const MathLib::vec3f & ambientColor);
        ~RenderQueue();

        void AddRenderableNode(MeshSceneNode * node);
        void SetCameraNode(CameraSceneNode * node);
        void SetSkyNode(SkySceneNode * node);
        void AddLightNode(LightSceneNode * node);

        RenderVector & GetRenderableNodes();
        const LightsVector & GetLights() const;

        CameraSceneNode * GetActiveCamera() const;
        SkySceneNode * GetActiveSky() const;

        MathLib::vec3f GetAmbientColor() const;

    private:
        RenderVector meshNodesVector;
        LightsVector lights;
        MathLib::vec3f ambientColor = { 0.0f, 0.0f, 0.0f };
        CameraSceneNode * activeCamera = nullptr;
        SkySceneNode * activeSky = nullptr;

    };
}
