#include "AnimatedSpriteSceneNode.h"

using namespace ShiftEngine;
using namespace MathLib;

//////////////////////////////////////////////////////////////////////////
// AnimationFrame

SpriteAnimation::AnimationFrame::AnimationFrame(const vec2f& leftTopCoords,
                                                const vec2f& rightBottomCoords,
                                                float duration)
    : leftTopCoords(leftTopCoords)
    , rightBottomCoords(rightBottomCoords)
    , duration(duration)
{
}

//////////////////////////////////////////////////////////////////////////
// SpriteAnimation

SpriteAnimation::SpriteAnimation(const std::vector<AnimationFrame> &frames)
    : frames(frames)
{
}

void SpriteAnimation::AddFrame(const vec2f& leftTopCoords,
                               const vec2f& rightBottomCoords,
                               float duration)
{
    frames.emplace_back(leftTopCoords, rightBottomCoords, duration);
    CalculateTotalAnimationTime();
}

float SpriteAnimation::GetTotalAnimationTime() const
{
    return totalAnimationTime;
}

void SpriteAnimation::CalculateTotalAnimationTime()
{
    totalAnimationTime = 0.0f;
    for (auto& frame : frames)
        totalAnimationTime += frame.duration;
}

const std::vector<SpriteAnimation::AnimationFrame>& SpriteAnimation::GetFrames() const
{
    return frames;
}

//////////////////////////////////////////////////////////////////////////
// AnimatedSpriteSceneNode

AnimatedSpriteSceneNode::AnimatedSpriteSceneNode(ITexturePtr &texture,
                                                 SceneGraph * sceneGraph,
                                                 int spriteLayer)
    : SpriteSceneNode(texture, sceneGraph, spriteLayer)
{
}

SpriteAnimation& AnimatedSpriteSceneNode::GetAnimationHandler()
{
    return animationHandler;
}

void AnimatedSpriteSceneNode::OnUpdate(double dt)
{
    static double elapsedTime = 0.0;
    elapsedTime += dt;
    if (elapsedTime > animationHandler.GetTotalAnimationTime())
        elapsedTime -= animationHandler.GetTotalAnimationTime();

    // get current frame and set it
    double calculatedTime = 0.0f;
    for (auto& frame : animationHandler.GetFrames())
    {
        calculatedTime += frame.duration;
        if (calculatedTime > elapsedTime)
        {
            SpriteSceneNode::CalculateTextureMatrix(frame.leftTopCoords, frame.rightBottomCoords);
            break;
        }
    }
}
