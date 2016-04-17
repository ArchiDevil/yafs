#include "CameraSceneNode.h"

#include "../RenderQueue.h"

#include <Utilities/logger.hpp>

ShiftEngine::CameraSceneNode::CameraSceneNode(float screenWidth, float screenHeight, float zNear, float zFar, float FOV, CameraViewType viewType)
    : ISceneNode()
    , zNear(zNear)
    , zFar(zFar)
    , fov(FOV)
    , screenWidth(screenWidth)
    , screenHeight(screenHeight)
    , viewType(viewType)
{
    MathLib::vec3f LOOK_POS = lookVector + position;
    matView = MathLib::matrixLookAtRH<float>(position, LOOK_POS, upVector);
    RebuildProjMatrix();
}

ShiftEngine::CameraSceneNode::CameraSceneNode(const MathLib::vec3f &position)
    : ISceneNode()
    , position(position)
{}

void ShiftEngine::CameraSceneNode::SetPosition(const MathLib::vec3f & pos)
{
    position = pos;
}

void ShiftEngine::CameraSceneNode::MoveUpDown(float units)
{
    position += upVector * units;
}

void ShiftEngine::CameraSceneNode::MoveLeftRight(float units)
{
    position += rightVector * units;
}

void ShiftEngine::CameraSceneNode::MoveForwardBackward(float units)
{
    position += lookVector * units;
}

void ShiftEngine::CameraSceneNode::Update()
{
    MathLib::vec3f eye = lookVector + position;
    matView = MathLib::matrixLookAtRH<float>(position, eye, upVector);
    frustum.BuildFrustum(matView, matProj);
}

MathLib::vec3f ShiftEngine::CameraSceneNode::GetLookVector() const
{
    return lookVector;
}

MathLib::vec3f ShiftEngine::CameraSceneNode::GetRightVector() const
{
    return rightVector;
}

MathLib::vec3f ShiftEngine::CameraSceneNode::GetPosition() const
{
    return position;
}

ShiftEngine::CameraFrustum * ShiftEngine::CameraSceneNode::GetFrustumPtr()
{
    return &frustum;
}

void ShiftEngine::CameraSceneNode::LookAt(const MathLib::vec3f & point)
{
    lookVector = MathLib::normalize(point);
}

void ShiftEngine::CameraSceneNode::PushToRQ(RenderQueue & /*rq*/)
{}

const MathLib::mat4f & ShiftEngine::CameraSceneNode::GetProjectionMatrix() const
{
    return matProj;
}

const MathLib::mat4f & ShiftEngine::CameraSceneNode::GetViewMatrix() const
{
    return matView;
}

MathLib::vec3f ShiftEngine::CameraSceneNode::GetUpVector() const
{
    return upVector;
}

float ShiftEngine::CameraSceneNode::GetZNear() const
{
    return zNear;
}

float ShiftEngine::CameraSceneNode::GetZFar() const
{
    return zFar;
}

float ShiftEngine::CameraSceneNode::GetFOV() const
{
    return fov;
}

void ShiftEngine::CameraSceneNode::SetZFar(float val)
{
    zFar = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetZNear(float val)
{
    zNear = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetFOV(float val)
{
    fov = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetScreenWidth(float val)
{
    screenWidth = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::SetScreenHeight(float val)
{
    screenHeight = val;
    RebuildProjMatrix();
}

void ShiftEngine::CameraSceneNode::RebuildProjMatrix()
{
    switch (viewType)
    {
    case ShiftEngine::CameraViewType::Projection:
        matProj = MathLib::matrixPerspectiveFovRH<float>(M_PIF * fov / 180.0f,       // vertical FoV
                                                         screenWidth / screenHeight, // screen rate
                                                         zNear,
                                                         zFar);
    case ShiftEngine::CameraViewType::Orthographic:
        matProj = MathLib::matrixOrthoRH<float>(screenWidth, screenHeight, zFar, zNear);
        break;
    default:
        LOG_FATAL_ERROR("Unable to recognize projection type");
        break;
    }
}

void ShiftEngine::CameraSceneNode::RotateByQuaternion(const MathLib::qaFloat & quat)
{
    // transform all vectors
    MathLib::vec3f look(lookVector.x, lookVector.y, lookVector.z);
    look = look * quat;
    lookVector = look;

    MathLib::vec3f up(upVector.x, upVector.y, upVector.z);
    up = up * quat;
    upVector = up;

    MathLib::vec3f right(rightVector.x, rightVector.y, rightVector.z);
    right = right * quat;
    rightVector = right;

    MathLib::vec3f LOOK_POS = lookVector + position;
    matView = MathLib::matrixLookAtRH<float>(position, LOOK_POS, upVector);
}

void ShiftEngine::CameraSceneNode::SetSphericalCoords(const MathLib::vec3f & center, float phi, float theta, float r)
{
    position = MathLib::GetPointOnSphere(center, r, phi, theta);
    lookVector = center - position;
    lookVector = normalize(lookVector);
    // we assume that UP vector is always direct to up
    upVector = {0.0f, 0.0f, 1.0f};
    rightVector = MathLib::cross(lookVector, upVector);
    rightVector = normalize(rightVector);
    upVector = MathLib::cross(rightVector, lookVector);
    upVector = normalize(upVector);

    MathLib::vec3f at = lookVector + position;
    matView = MathLib::matrixLookAtRH<float>(position, at, upVector);
}

MathLib::AABB ShiftEngine::CameraSceneNode::GetBBox() const
{
    return MathLib::AABB();
}
