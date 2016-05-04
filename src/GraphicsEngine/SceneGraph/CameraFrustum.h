#pragma once

#include <MathLib/math.h>

namespace ShiftEngine
{
class CameraFrustum
{
public:
    enum class CullingStatus
    {
        CS_Out,
        CS_Intersect,
        CS_In
    };

    CameraFrustum() = default;
    CameraFrustum(const CameraFrustum &) = default;
    ~CameraFrustum() = default;

    void BuildFrustum(const MathLib::mat4f & matView, const MathLib::mat4f & matProj);

    CullingStatus CheckAABB(const MathLib::AABB & bbox) const;
    CullingStatus CheckOOBB(const MathLib::OOBB & bbox) const;
    CullingStatus CheckQTreeNode(const MathLib::AABB & bbox) const;

    //not implemented
    bool CheckPoint(const MathLib::vec3f & point) const;
    bool CheckBox(const MathLib::vec3f & center, const MathLib::vec3f & radius) const;
    bool CheckSphere(const MathLib::vec3f & position, const MathLib::vec3f & radius) const;
    bool CheckRectangle(const MathLib::vec3f & position, const MathLib::vec3f & sizes) const;

private:
    MathLib::plane<float> planes[6]; //6 плоскостей - передн€€, задн€€, 2 боковые, 2 верхн€€/нижн€€
};

}
