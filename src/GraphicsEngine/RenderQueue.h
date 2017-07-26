#pragma once

#include <MathLib/math.h>

#include <Utilities/IManager.h>

#include <vector>
#include <map>

//This class provides storage for scene nodes that will be rendered by renderer.
namespace ShiftEngine
{
class ISceneNode;
class CameraSceneNode;
class SpriteSceneNode;
class TextSceneNode;

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
using SpritesVault = std::map<int, std::vector<SpriteSceneNode*>, greater<int>>;

class RenderQueue : public IManager
{
public:
    RenderQueue(const MathLib::vec3f & ambientColor);

    void SetCameraNode(CameraSceneNode * node);
    void AddSpriteNode(SpriteSceneNode * node, int renderingLayer);
    void AddTextNode(TextSceneNode * node);

    SpritesVault& GetSpriteNodes();
    const std::vector<TextSceneNode*> GetTextNodes();
    CameraSceneNode& GetActiveCamera() const;
    MathLib::vec3f GetAmbientColor() const;

private:
    SpritesVault sprites;
    std::vector<TextSceneNode*> textNodes;

    MathLib::vec3f ambientColor = {0.0f, 0.0f, 0.0f};
    CameraSceneNode * activeCamera = nullptr;

};
}
