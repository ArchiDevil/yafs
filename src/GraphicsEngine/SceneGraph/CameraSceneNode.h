#pragma once

#include "ISceneNode.h"
#include "CameraFrustum.h"

#include <MathLib/math/quaternion.h>

#include <memory>

namespace ShiftEngine
{

static const float MAX_ANGLE = 89.5f;

enum class CameraViewType
{
    Perspective,
    Orthographic
};

class CameraSceneNode final : public ISceneNode
{
public:
    CameraSceneNode(float screenWidth, float screenHeight, float zNear, float zFar, float FOV, CameraViewType viewType, SceneGraph * sceneGraph);

    // Inherited from ISceneNode
    void SetLocalPosition(const MathLib::vec3f & pos) override;
    void RotateByLocalQuaternion(const MathLib::qaFloat & val) override;
    void PushToRQ(RenderQueue & rq) override;
    MathLib::aabb3f GetBBox() const override;

    void Update();
    void MoveUpDown(float units);
    void MoveLeftRight(float units);
    void MoveForwardBackward(float units);
    void LookAt(const MathLib::vec3f & point);
    void SetSphericalCoords(const MathLib::vec3f & lookPoint, float phi, float theta, float r);

    const MathLib::vec3f & GetLookVector() const;
    const MathLib::vec3f & GetRightVector() const;
    const MathLib::vec3f & GetUpVector() const;
    const CameraFrustum & GetFrustum() const;
    const MathLib::mat4f & GetProjectionMatrix() const;
    const MathLib::mat4f & GetViewMatrix() const;

    void SetZNear(float val);
    float GetZNear() const;

    void SetZFar(float val);
    float GetZFar() const;

    void SetFOV(float val);
    float GetFOV() const;

    void SetScreenWidth(float val);
    void SetScreenHeight(float val);

private:
    void RebuildProjMatrix();

    MathLib::mat4f matView;
    MathLib::mat4f matProj;

    float zNear = 0.1f;
    float zFar = 100.0f;
    float fov = 60.0f;
    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    CameraViewType viewType = CameraViewType::Perspective;
    CameraFrustum frustum;

    float viewAngle = 0.0f;
    MathLib::vec3f angles = {0.0f, 0.0f, 0.0f};
    MathLib::vec3f upVector = {0.0f, 1.0f, 0.0f};
    MathLib::vec3f lookVector = {0.0f, 0.0f, 1.0f};
    MathLib::vec3f rightVector = {1.0f, 0.0f, 0.0f};

};

}
