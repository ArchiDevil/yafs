#pragma once

#include <MathLib/math.h>

#include <vector>
#include <map>

//This class provides storage for scene nodes that will be rendered by renderer.
namespace ShiftEngine
{
class ISceneNode;
class MeshSceneNode;
class CameraSceneNode;
class SkySceneNode;
class LightSceneNode;
class SpriteSceneNode;

// custom functor to sort sprite layers from high to low 
// in map to overdraw sprites with bigger layer number
template<class T>
struct greater
{
    bool operator()(const T& left, const T& right) const
    {
        return (left > right);
    }
};

// remove this shit with huge amount of allocations
using RenderVector = std::vector<MeshSceneNode*>;
using LightsVector = std::vector<LightSceneNode*>;
using SpritesVault = std::map<int, std::vector<SpriteSceneNode*>, greater<int>>;

class RenderQueue
{
public:
    RenderQueue(const MathLib::vec3f & ambientColor);
    ~RenderQueue();

    void AddRenderableNode(MeshSceneNode * node);
    void SetCameraNode(CameraSceneNode * node);
    void SetSkyNode(SkySceneNode * node);
    void AddLightNode(LightSceneNode * node);
    void AddSpriteNode(SpriteSceneNode * node, int renderingLayer);

    RenderVector & GetRenderableNodes();
    SpritesVault & GetSpriteNodes();
    const LightsVector & GetLights() const;

    CameraSceneNode * GetActiveCamera() const;
    SkySceneNode * GetActiveSky() const;

    MathLib::vec3f GetAmbientColor() const;

private:
    RenderVector meshes;
    LightsVector lights;
    SpritesVault sprites;

    MathLib::vec3f ambientColor = {0.0f, 0.0f, 0.0f};
    CameraSceneNode * activeCamera = nullptr;
    SkySceneNode * activeSky = nullptr;

};
}
