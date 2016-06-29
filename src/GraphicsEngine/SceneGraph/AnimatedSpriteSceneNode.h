#pragma once

#include "SpriteSceneNode.h"

namespace ShiftEngine
{

class SpriteAnimation
{
public:
    struct AnimationFrame
    {
        AnimationFrame(const MathLib::vec2f& leftTopCoords,
                       const MathLib::vec2f& rightBottomCoords,
                       float duration);

        const MathLib::vec2f leftTopCoords;
        const MathLib::vec2f rightBottomCoords;
        const float duration;
    };

    SpriteAnimation() = default;
    SpriteAnimation(const std::vector<AnimationFrame> &frames);

    SpriteAnimation(const SpriteAnimation&) = delete;
    SpriteAnimation(SpriteAnimation&&) = delete;
    SpriteAnimation& operator=(const SpriteAnimation&) = delete;
    SpriteAnimation& operator=(SpriteAnimation&&) = delete;

    void AddFrame(const MathLib::vec2f& leftTopCoords,
                  const MathLib::vec2f& rightBottomCoords,
                  float duration);
    float GetTotalAnimationTime() const;
    const std::vector<AnimationFrame>& GetFrames() const;

private:
    void CalculateTotalAnimationTime();

    float totalAnimationTime = 0.0;
    std::vector<AnimationFrame> frames;
};

class AnimatedSpriteSceneNode : public SpriteSceneNode
{
public:
    AnimatedSpriteSceneNode(ITexturePtr &texture, 
                            SceneGraph * sceneGraph,
                            int spriteLayer);
    SpriteAnimation& GetAnimationHandler();
    void OnUpdate(double dt) override;

protected:
    SpriteAnimation animationHandler;

};

}
