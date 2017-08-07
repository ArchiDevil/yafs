#include "CppUnitTest.h"
#include "../math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MathLib;

namespace MathLibTests
{
namespace IntersectionTests
{
TEST_CLASS(MathLibIntersectionTests)
{
public:
    TEST_METHOD(AABB2Tests)
    {
        aabb2f first{ { 0.0f, 0.0f }, { 1.0f, 1.0f } };

        aabb2f second1{ { 0.5f, 0.0f }, { 1.0f, 1.0f } };
        aabb2f second2{ { -0.5f, 0.0f }, { 1.0f, 1.0f } };
        aabb2f second3{ { 0.0f, 0.5f }, { 1.0f, 1.0f } };
        aabb2f second4{ { 0.0f, -0.5f }, { 1.0f, 1.0f } };

        Assert::IsTrue(intersections::AABBsIntersection(first, second1));
        Assert::IsTrue(intersections::AABBsIntersection(first, second2));
        Assert::IsTrue(intersections::AABBsIntersection(first, second3));
        Assert::IsTrue(intersections::AABBsIntersection(first, second4));

        aabb2f third{ {2.0f, 2.0f}, {1.0f, 1.0f} };
        Assert::IsFalse(intersections::AABBsIntersection(first, third));
    }

    TEST_METHOD(AABB2SphereTests)
    {
        aabb2f first{ {0.0f, 0.0f}, {1.0f, 1.0f} };

        Assert::IsTrue(intersections::AABBSphereIntersection(first, { 0.0f, 0.0f }, 1.0f));

        Assert::IsFalse(intersections::AABBSphereIntersection(first, { 2.0f, 2.0f }, 1.0f));
        Assert::IsFalse(intersections::AABBSphereIntersection(first, { 2.0f, 2.0f }, 1.2f));
        Assert::IsTrue(intersections::AABBSphereIntersection(first, { 2.0f, 2.0f }, 1.5f));

        Assert::IsFalse(intersections::AABBSphereIntersection(first, { -2.0f, 2.0f }, 1.0f));
        Assert::IsFalse(intersections::AABBSphereIntersection(first, { -2.0f, 2.0f }, 1.2f));
        Assert::IsTrue(intersections::AABBSphereIntersection(first, { -2.0f, 2.0f }, 1.5f));

        Assert::IsFalse(intersections::AABBSphereIntersection(first, { 2.0f, -2.0f }, 1.0f));
        Assert::IsFalse(intersections::AABBSphereIntersection(first, { 2.0f, -2.0f }, 1.2f));
        Assert::IsTrue(intersections::AABBSphereIntersection(first, { 2.0f, -2.0f }, 1.5f));

        Assert::IsFalse(intersections::AABBSphereIntersection(first, { -2.0f, -2.0f }, 1.0f));
        Assert::IsFalse(intersections::AABBSphereIntersection(first, { -2.0f, -2.0f }, 1.2f));
        Assert::IsTrue(intersections::AABBSphereIntersection(first, { -2.0f, -2.0f }, 1.5f));
    }

    TEST_METHOD(AABB2PointTests)
    {
        aabb2f first{ {0.0f, 0.0f}, {1.0f, 1.0f} };
        Assert::IsTrue(intersections::AABBPointIntersection(first, { 0.0f, 0.0f }));

        Assert::IsTrue(intersections::AABBPointIntersection(first, { 0.5f, 0.0f }));
        Assert::IsTrue(intersections::AABBPointIntersection(first, { 0.0f, 0.5f }));
        Assert::IsTrue(intersections::AABBPointIntersection(first, { -0.5f, 0.0f }));
        Assert::IsTrue(intersections::AABBPointIntersection(first, { 0.0f, -0.5f }));

        Assert::IsFalse(intersections::AABBPointIntersection(first, { 1.1f, 1.1f }));
        Assert::IsFalse(intersections::AABBPointIntersection(first, { 1.1f, -1.1f }));
        Assert::IsFalse(intersections::AABBPointIntersection(first, { -1.1f, 1.1f }));
        Assert::IsFalse(intersections::AABBPointIntersection(first, { -1.1f, -1.1f }));
    }

    TEST_METHOD(AABB3Tests)
    {
        aabb3f first{ { -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } };

        aabb3f second1{ { 0.0f, -0.5f, -0.5f }, { 1.0f, 0.5f, 0.5f } };
        aabb3f second2{ { -1.0f, -0.5f, -0.5f }, { 0.0f, 0.5f, 0.5f } };

        aabb3f second3{ { -0.5f, 0.0f, -0.5f }, { 0.5f, 1.0f, 0.5f } };
        aabb3f second4{ { 0.0f, -1.0f, -0.5f }, { 1.0f, 0.0f, 0.5f } };

        aabb3f second5{ { -0.5f, -0.5f, 0.0f }, { 0.5f, 0.5f, 1.0f } };
        aabb3f second6{ { -0.5f, -0.5f, -1.0f }, { 0.5f, 0.5f, 0.0f } };

        Assert::IsTrue(intersections::AABBsIntersection(first, second1));
        Assert::IsTrue(intersections::AABBsIntersection(first, second2));
        Assert::IsTrue(intersections::AABBsIntersection(first, second3));
        Assert::IsTrue(intersections::AABBsIntersection(first, second4));
        Assert::IsTrue(intersections::AABBsIntersection(first, second5));
        Assert::IsTrue(intersections::AABBsIntersection(first, second6));

        aabb3f third{ { 2.0f, 2.0f, 2.0f }, { 3.0f, 3.0f, 3.0f } };
        Assert::IsFalse(intersections::AABBsIntersection(first, third));
    }

    TEST_METHOD(AABBOOBBTests)
    {
        aabb2f first{ {0.0f, 0.0f}, {1.0f, 1.0f} };
        oobb2f second{ {2.0f, 2.0f}, {1.0f, 1.0f}, 0.7f };
        Assert::IsFalse(intersections::AABBOOBBIntersection(first, second));

        oobb2f third{ {1.0f, 1.0f}, {1.0f, 1.0f}, 0.25f };
        Assert::IsTrue(intersections::AABBOOBBIntersection(first, third));

        first = { { 10.0f, 10.0f },{ 1.0f, 1.0f } };
        second = { { 12.0f, 12.0f },{ 1.0f, 1.0f }, 0.7f };
        Assert::IsFalse(intersections::AABBOOBBIntersection(first, second));

        third = { { 11.0f, 11.0f },{ 1.0f, 1.0f }, 0.25f };
        Assert::IsTrue(intersections::AABBOOBBIntersection(first, third));
    }

    TEST_METHOD(OOBBSphereTests)
    {
        oobb2f first{ {0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f };
        Assert::IsTrue(intersections::OOBBSphereIntersection(first, { 0.0f, 0.0f }, 1.0f));
        Assert::IsTrue(intersections::OOBBSphereIntersection(first, { 1.0f, 0.0f }, 1.0f));
        Assert::IsTrue(intersections::OOBBSphereIntersection(first, { 0.0f, 1.0f }, 1.0f));

        Assert::IsFalse(intersections::OOBBSphereIntersection(first, { 3.0f, 3.0f }, 1.0f));
        Assert::IsFalse(intersections::OOBBSphereIntersection(first, { -3.0f, 3.0f }, 1.0f));
        Assert::IsFalse(intersections::OOBBSphereIntersection(first, { -3.0f, -3.0f }, 1.0f));
        Assert::IsFalse(intersections::OOBBSphereIntersection(first, { 3.0f, -3.0f }, 1.0f));
    }

    TEST_METHOD(OOBBPointTests)
    {
        oobb2f first{ {0.0f, 0.0f}, {1.0f, 1.0f}, 0.707f };
        Assert::IsTrue(intersections::OOBBPointIntersection(first, { 0.0f, 0.0f }));

        Assert::IsTrue(intersections::OOBBPointIntersection(first, { 1.0f, 0.0f }));
        Assert::IsTrue(intersections::OOBBPointIntersection(first, { 0.0f, 1.0f }));
        Assert::IsTrue(intersections::OOBBPointIntersection(first, { -1.0f, 0.0f }));
        Assert::IsTrue(intersections::OOBBPointIntersection(first, { 0.0f, -1.0f }));
    }

    TEST_METHOD(SphereTests)
    {
        Assert::IsTrue(intersections::SphereSphereIntersection(vec2f{ 0.0f, 0.0f }, vec2f{ 1.0f, 0.0f }, 1.0f, 1.0f));
        Assert::IsFalse(intersections::SphereSphereIntersection(vec2f{ 0.0f, 0.0f }, vec2f{ 3.0f, 0.0f }, 1.0f, 1.0f));

        Assert::IsTrue(intersections::SphereSphereIntersection(vec3f{ 1.0f, 0.0f, 0.0f }, vec3f{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f));
        Assert::IsFalse(intersections::SphereSphereIntersection(vec3f{ 1.0f, 2.0f, 15.0f }, vec3f{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f));
    }

    TEST_METHOD(RayTests)
    {
        //ray-bbox intersection test
        aabb3f bbox{ {10.0f, -15.0f, -15.0f}, {20.0f, 15.0f, 15.0f} };
        ray3f r{ {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} };
        Assert::IsTrue(intersections::RayAABBIntersection(r, bbox, 0.0f, 100.0f));

        r.direction = vec3f(-1.0f, 0.0f, 0.0f);
        Assert::IsFalse(intersections::RayAABBIntersection(r, bbox, 0.0f, 100.0f));

        //ray-sphere intersection test
        r.direction = vec3f(1.0f, 0.0f, 0.0f);
        Assert::IsTrue(intersections::RaySphereIntersection(r, { 10.0f, 0.0f, 0.0f }, 1.0f));

        Assert::IsFalse(intersections::RaySphereIntersection(r, { -10.0f, 0.0f, 0.0f }, 1.0f));
        Assert::IsFalse(intersections::RaySphereIntersection(r, { 10.0f, 10.0f, 0.0f }, 1.0f));
    }
};
}
}
