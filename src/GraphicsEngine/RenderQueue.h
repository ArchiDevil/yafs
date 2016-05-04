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
    class SpriteSceneNode;

    // remove this shit with huge amount of allocations
    using RenderVector = std::vector<MeshSceneNode*>;
    using LightsVector = std::vector<LightSceneNode*>;
    using SpritesVector = std::vector<SpriteSceneNode*>;

    class RenderQueue
    {
    public:
        RenderQueue(const MathLib::vec3f & ambientColor);
        ~RenderQueue();

        void AddRenderableNode(MeshSceneNode * node);
        void SetCameraNode(CameraSceneNode * node);
        void SetSkyNode(SkySceneNode * node);
        void AddLightNode(LightSceneNode * node);
        void AddSpriteNode(SpriteSceneNode * node);

        RenderVector & GetRenderableNodes();
        SpritesVector & GetSpriteNodes();
        const LightsVector & GetLights() const;

        CameraSceneNode * GetActiveCamera() const;
        SkySceneNode * GetActiveSky() const;

        MathLib::vec3f GetAmbientColor() const;

    private:
        RenderVector meshes;
        LightsVector lights;
        SpritesVector sprites;

        MathLib::vec3f ambientColor = { 0.0f, 0.0f, 0.0f };
        CameraSceneNode * activeCamera = nullptr;
        SkySceneNode * activeSky = nullptr;

    };
}
