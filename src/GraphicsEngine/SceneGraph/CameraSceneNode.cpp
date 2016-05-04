#include "CameraSceneNode.h"

#include "../RenderQueue.h"

#include <Utilities/logger.hpp>

using namespace ShiftEngine;
using namespace MathLib;

CameraSceneNode::CameraSceneNode(float screenWidth, float screenHeight, float zNear, float zFar, float FOV, CameraViewType viewType, SceneGraph * sceneGraph)
    : ISceneNode(sceneGraph)
    , zNear(zNear)
    , zFar(zFar)
    , fov(FOV)
    , screenWidth(screenWidth)
    , screenHeight(screenHeight)
    , viewType(viewType)
{
    vec3f LOOK_POS = lookVector + position;
    matView = matrixLookAtRH<float>(position, LOOK_POS, upVector);
    RebuildProjMatrix();
}

void CameraSceneNode::SetLocalPosition(const vec3f & pos)
{
    position = pos;
}

void CameraSceneNode::MoveUpDown(float units)
{
    position += upVector * units;
}

void CameraSceneNode::MoveLeftRight(float units)
{
    position += rightVector * units;
}

void CameraSceneNode::MoveForwardBackward(float units)
{
    position += lookVector * units;
}

void CameraSceneNode::Update()
{
    vec3f eye = lookVector + position;
    matView = matrixLookAtRH<float>(position, eye, upVector);
    frustum.BuildFrustum(matView, matProj);
}

vec3f CameraSceneNode::GetLookVector() const
{
    return lookVector;
}

vec3f CameraSceneNode::GetRightVector() const
{
    return rightVector;
}

vec3f CameraSceneNode::GetWorldPosition() const
{
    return position;
}

CameraFrustum * CameraSceneNode::GetFrustumPtr()
{
    return &frustum;
}

void CameraSceneNode::LookAt(const vec3f & point)
{
    lookVector = normalize(point);
}

void CameraSceneNode::PushToRQ(RenderQueue & /*rq*/)
{}

const mat4f & CameraSceneNode::GetProjectionMatrix() const
{
    return matProj;
}

const mat4f & CameraSceneNode::GetViewMatrix() const
{
    return matView;
}

vec3f CameraSceneNode::GetUpVector() const
{
    return upVector;
}

float CameraSceneNode::GetZNear() const
{
    return zNear;
}

float CameraSceneNode::GetZFar() const
{
    return zFar;
}

float CameraSceneNode::GetFOV() const
{
    return fov;
}

void CameraSceneNode::SetZFar(float val)
{
    zFar = val;
    RebuildProjMatrix();
}

void CameraSceneNode::SetZNear(float val)
{
    zNear = val;
    RebuildProjMatrix();
}

void CameraSceneNode::SetFOV(float val)
{
    fov = val;
    RebuildProjMatrix();
}

void CameraSceneNode::SetScreenWidth(float val)
{
    screenWidth = val;
    RebuildProjMatrix();
}

void CameraSceneNode::SetScreenHeight(float val)
{
    screenHeight = val;
    RebuildProjMatrix();
}

void CameraSceneNode::RebuildProjMatrix()
{
    switch (viewType)
    {
    case CameraViewType::Projection:
        matProj = matrixPerspectiveFovRH<float>(M_PIF * fov / 180.0f,       // vertical FoV
                                                screenWidth / screenHeight, // screen rate
                                                zNear,
                                                zFar);
    case CameraViewType::Orthographic:
        matProj = matrixOrthoRH<float>(screenWidth, screenHeight, zFar, zNear);
        break;
    default:
        LOG_FATAL_ERROR("Unable to recognize projection type");
        break;
    }
}

void CameraSceneNode::RotateByQuaternion(const qaFloat & quat)
{
    // transform all vectors
    vec3f look = {lookVector.x, lookVector.y, lookVector.z};
    look = look * quat;
    lookVector = look;

    vec3f up = {upVector.x, upVector.y, upVector.z};
    up = up * quat;
    upVector = up;

    vec3f right = {rightVector.x, rightVector.y, rightVector.z};
    right = right * quat;
    rightVector = right;

    vec3f LOOK_POS = lookVector + position;
    matView = matrixLookAtRH<float>(position, LOOK_POS, upVector);
}

void CameraSceneNode::SetSphericalCoords(const vec3f & center, float phi, float theta, float r)
{
    position = GetPointOnSphere(center, r, phi, theta);
    lookVector = center - position;
    lookVector = normalize(lookVector);
    // we assume that UP vector is always direct to up
    upVector = {0.0f, 0.0f, 1.0f};
    rightVector = cross(lookVector, upVector);
    rightVector = normalize(rightVector);
    upVector = cross(rightVector, lookVector);
    upVector = normalize(upVector);

    vec3f at = lookVector + position;
    matView = matrixLookAtRH<float>(position, at, upVector);
}

AABB CameraSceneNode::GetBBox() const
{
    return {};
}
