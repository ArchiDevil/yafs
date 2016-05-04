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
    Projection,
    Orthographic
};

class CameraSceneNode : public ISceneNode
{
public:
    CameraSceneNode(float screenWidth, float screenHeight, float zNear, float zFar, float FOV, CameraViewType viewType, SceneGraph * sceneGraph);

    void SetLocalPosition(const MathLib::vec3f & pos);
    void Update();
    void MoveUpDown(float units);
    void MoveLeftRight(float units);
    void MoveForwardBackward(float units);
    void RotateByQuaternion(const MathLib::qaFloat & quat);
    void LookAt(const MathLib::vec3f & point);
    void SetSphericalCoords(const MathLib::vec3f & lookPoint, float phi, float theta, float r);

    MathLib::vec3f GetLookVector() const;
    MathLib::vec3f GetRightVector() const;
    MathLib::vec3f GetWorldPosition() const;
    MathLib::vec3f GetUpVector() const;

    CameraFrustum * GetFrustumPtr();
    const CameraFrustum * GetFrustumPtr() const;

    virtual void PushToRQ(RenderQueue & rq) override;

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

    virtual MathLib::AABB GetBBox() const override;

private:
    void RebuildProjMatrix();

    MathLib::mat4f matView;
    MathLib::mat4f matProj;

    float zNear = 0.1f;
    float zFar = 100.0f;
    float fov = 60.0f;
    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    CameraViewType viewType = CameraViewType::Projection;
    CameraFrustum frustum;

    float viewAngle = 0.0f;
    MathLib::vec3f angles = {0.0f, 0.0f, 0.0f};
    MathLib::vec3f upVector = {0.0f, 0.0f, 1.0f};
    MathLib::vec3f lookVector = {0.0f, 1.0f, 0.0f};
    MathLib::vec3f position = {0.0f, 0.0f, 0.0f};
    MathLib::vec3f rightVector = {1.0f, 0.0f, 0.0f};

};

}
