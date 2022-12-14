#include "CppUnitTest.h"
#include "../math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MathLib;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::vec3f>(const MathLib::vec3f& t)
{
    std::wostringstream s; s << t.x << " " << t.y << " " << t.z << "\n"; return s.str();
}

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::qaFloat>(const MathLib::qaFloat& t)
{
    std::wostringstream s; s << t.vector.x << " " << t.vector.y << " " << t.vector.z << " " << t.w; return s.str();
}

namespace UnitTests
{
    TEST_CLASS(MathLibUnitTests)
    {
    public:
        TEST_METHOD(AABBTests)
        {
            AABB aabb = AABB({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
            Assert::AreEqual(aabb.GetVolume(), 1.0f);
            Assert::AreEqual(aabb.IntersectsAABB(AABB({ -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f })), true);
            aabb.Include(AABB({ -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f }));
            Assert::AreEqual(aabb.bMin, { -0.5f, -0.5f, -0.5f });
            Assert::AreEqual(aabb.bMax, { 1.0f, 1.0f, 1.0f });
        }

        TEST_METHOD(OOBBTests_NOT_IMPLEMENTED)
        {
        }

        TEST_METHOD(Vector2Tests)
        {
            vec2<float> testVec(3.0f, 2.0f);
            Assert::AreEqual(3.0f, testVec.x);
            Assert::AreEqual(2.0f, testVec.y);

            testVec.x = 1.0f;
            testVec.y = 0.0f;
            Assert::AreEqual(1.0f, testVec.length());

            Assert::IsTrue(testVec == vec2<float>(1.0f, 0.0f));
            Assert::IsTrue(testVec != vec2<float>(1.0f, 1.0f));

            testVec = vec2<float>(22.2f, 0.0f);
            Assert::AreEqual(22.2f, testVec.length());

            vec2<float> secVec = vec2<float>(12.2f, -2.0f);
            testVec += secVec;
            Assert::IsTrue(testVec == vec2<float>(34.4f, -2.0f));

            testVec -= secVec;
            Assert::IsTrue(testVec == vec2<float>(22.2f, 0.0f));

            testVec *= 2.0f;
            Assert::IsTrue(testVec == vec2<float>(44.4f, 0.0f));
        }

        TEST_METHOD(Vector3Tests)
        {
            vec3<float> testVec(3.0f, 2.0f, -1.0f);
            Assert::AreEqual(3.0f, testVec.x);
            Assert::AreEqual(2.0f, testVec.y);
            Assert::AreEqual(-1.0f, testVec.z);

            testVec.x = 1.0f;
            testVec.y = 0.0f;
            testVec.z = 0.0f;
            Assert::AreEqual(1.0f, testVec.length());

            Assert::IsTrue(testVec == vec3<float>(1.0f, 0.0f, 0.0f));
            Assert::IsTrue(testVec != vec3<float>(1.0f, 1.0f, 0.0f));

            vec3<float> secVec = vec3<float>(12.2f, -2.0f, 51.0f);
            testVec += secVec;
            Assert::IsTrue(testVec == vec3<float>(13.2f, -2.0f, 51.0f));

            testVec -= secVec;
            Assert::IsTrue(testVec == vec3<float>(1.0f, 0.0f, 0.0f));

            testVec *= 2.0f;
            Assert::IsTrue(testVec == vec3<float>(2.0f, 0.0f, 0.0f));
        }

        TEST_METHOD(Vector4Tests)
        {
            vec4<float> testVec(3.0f, 2.0f, -1.0f, -8.0f);
            Assert::AreEqual(3.0f, testVec.x);
            Assert::AreEqual(2.0f, testVec.y);
            Assert::AreEqual(-1.0f, testVec.z);
            Assert::AreEqual(-8.0f, testVec.w);

            testVec.x = 1.0f;
            testVec.y = 0.0f;
            testVec.z = 0.0f;
            testVec.w = 0.0f;
            Assert::AreEqual(1.0f, testVec.length());

            Assert::IsTrue(testVec == vec4<float>(1.0f, 0.0f, 0.0f, 0.0f));
            Assert::IsTrue(testVec != vec4<float>(1.0f, 1.0f, 0.0f, 1.0f));

            vec4<float> secVec = vec4<float>(12.2f, -2.0f, 51.0f, -168.6f);
            testVec += secVec;
            Assert::IsTrue(testVec == vec4<float>(13.2f, -2.0f, 51.0f, -168.6f));

            testVec -= secVec;
            Assert::IsTrue(testVec == vec4<float>(1.0f, 0.0f, 0.0f, 0.0f));

            testVec *= 2.0f;
            Assert::IsTrue(testVec == vec4<float>(2.0f, 0.0f, 0.0f, 0.0f));
        }

        TEST_METHOD(PlaneTests)
        {
            plane<float> p = { 0.66f, 0.0f, 0.0f, 1.0f };
            p = normalize(p);
            Assert::IsTrue(p.d > 1.5f); // normalized
            Assert::IsTrue(abs(p.a - 1.0f) < 0.0001f);
            Assert::IsTrue(abs(p.b - 0.0f) < 0.0001f);
            Assert::IsTrue(abs(p.c - 0.0f) < 0.0001f);

            plane<double> dp1 = { 0.0,  1.0,  2.0, 3.0 };
            plane<double> dp2 = { 0.0,  1.0,  2.0,  3.0 };
            plane<double> dp3 = { 0.0, -1.0, -2.0, -3.0 };

            Assert::IsTrue(dp1 == dp2);
            Assert::IsTrue(dp2 != dp3);
            Assert::IsTrue(dp1 != dp3);

            Assert::IsTrue(planeDotNormal(p, vec3<float>(0.5f, 0.5f, 0.0f)) > 0.0f);
            Assert::IsTrue(planeDotNormal(p, vec3<float>(-0.5f, -0.5f, 0.0f)) < 0.0f);

            Assert::IsTrue(planeDotCoord(p, vec3<float>(3.0f, 0.0f, 0.0f)) > 0.0f);
            Assert::IsTrue(planeDotCoord(p, vec3<float>(-3.0f, 0.0f, 0.0f)) < 0.0f);
        }

        TEST_METHOD(MathLibTest_NOT_IMPLEMENTED)
        {
        }

        TEST_METHOD(IntersectionsTests)
        {
            //sphere-sphere intersection test
            bool result = SphereSphereIntersect(vec3f(1.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
            Assert::AreEqual(result, true);

            result = SphereSphereIntersect(vec3f(1.0f, 2.0f, 15.0f), vec3f(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
            Assert::AreEqual(result, false);

            //ray-bbox intersection test
            AABB bbox(vec3f(10.0f, -15.0f, -15.0f), vec3f(20.0f, 15.0f, 15.0f));
            Ray r(vec3f(0.0f, 0.0f, 0.0f), vec3f(1.0f, 0.0f, 0.0f));
            result = RayBoxIntersect(r, bbox, 0.0f, 100.0f);
            Assert::AreEqual(result, true);

            r.Direction = vec3f(-1.0f, 0.0f, 0.0f);
            result = RayBoxIntersect(r, bbox, 0.0f, 100.0f);
            Assert::AreEqual(result, false);

            //ray-sphere intersection test
            result = RaySphereIntersect(r, vec3f(10.0f, 0.0f, 0.0f), 1.0f);
            Assert::AreEqual(result, false);

            r.Direction = vec3f(1.0f, 0.0f, 0.0f);
            result = RaySphereIntersect(r, vec3f(10.0f, 0.0f, 0.0f), 1.0f);
            Assert::AreEqual(result, true);
        }

        TEST_METHOD(QuaternionTests)
        {
            // check operators
            const qaFloat qtOp1 = quaternionFromVecAngle(vec3<float>(1.0f, 0.0f, 0.0f), degrad(30.0f));
            const qaFloat qtOp2 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(90.0f));
            const float eps = std::numeric_limits<float>::epsilon();

            qaFloat result;

            vec3<float> vector(1.0f, 0.0f, 0.0f);
            matrix<float, 3> mat = qtOp2.to_matrix();
            vec3<float> resVector = mat * vector;
            Assert::AreEqual(vec3<float>(0.0f, 0.0f, 1.0f), resVector, L"Converting into matrix is not correct");

            qaFloat q1 = quaternionFromVecAngle(vec3<float>(0.0f, 0.0f, 1.0f), degrad(30.0f));
            qaFloat q2 = quaternionFromVecAngle(vec3<float>(0.0f, 0.0f, 1.0f), degrad(-30.0f));
            q1 = q1.conjugate();
            Assert::AreEqual(q1, q2, L"Conjugation is not valid");

            resVector = vector * qtOp2;
            Assert::AreEqual(vec3<float>(0.0f, 0.0f, -1.0f), resVector, L"Transforming of vector is not correct");

            mat = qtOp1.to_matrix();
            result = quaternionFromMatrix(mat);
            vec3<float> differenceVector = result.vector - qtOp1.vector;
            if (differenceVector.x > eps ||
                differenceVector.y > eps ||
                differenceVector.z > eps)
                Assert::Fail(L"Matrix transformations are not valid");

            const qaFloat qtOpPlus1 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(90.0f));
            const qaFloat qtOpPlus2 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(0.0f));
            const qaFloat divQtOpPlus = qtOpPlus1 + qtOpPlus2;
            resVector = vector * divQtOpPlus;
            float sin_angle = sin(degrad(45.0f));
            if (resVector.x - sin_angle > eps || 
                -resVector.z - sin_angle > eps)
                Assert::Fail(L"Plus operator is incorrect");

            const qaFloat divQtOpMinus = qtOpPlus1 - qtOpPlus2;
            resVector = vector * divQtOpMinus;
            if (resVector.x - sin_angle > eps || 
                -resVector.z - sin_angle > eps)
                Assert::Fail(L"Minus operator is incorrect");

            const qaFloat qtOpMult1 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(90.0f));
            const qaFloat qtOpMult2 = quaternionFromVecAngle(vec3<float>(1.0f, 0.0f, 0.0f), degrad(90.0f));
            resVector = vector * (qtOpMult1 * qtOpMult2);
            if (resVector.z + 1.0f > eps)
                Assert::Fail(L"Mult operator is incorrect");
        }

        TEST_METHOD(LineTests)
        {
            const float eps = std::numeric_limits<float>::epsilon();
            line2d<float> p = lineFromTwoPoints<float>({ 2, 3 } , { 1, 7 });

            if (std::abs(p.a -  4.0f) > eps ||
                std::abs(p.b -  1.0f) > eps || 
                std::abs(p.c - -11.0f) > eps)
                Assert::Fail(L"Line from two points computed incorrectly");

            Assert::IsTrue(p.direction() == vec2f{ -1.0f, 4.0f }, L"direction of line is computed incorrect");
            Assert::IsTrue(p.normal() == vec2f{ 4.0f, 1.0f }, L"normal of line is computed incorrect");

            p = lineFromPointAndDirectionVector<float>({ 1.0f, 2.0f }, { 2.0f, 1.0f });
            if (std::abs(p.a -  1.0f) > eps ||
                std::abs(p.b - -2.0f) > eps ||
                std::abs(p.c -  3.0f) > eps)
                Assert::Fail(L"Line from point and direction vector computed incorrectly");

            p = lineFromPointAndNormalVector<float>({ -1.0f, -3.0f }, {3.0f, -1.0f});
            if (std::abs(p.a -  3.0f) > eps ||
                std::abs(p.b - -1.0f) > eps ||
                std::abs(p.c -  0.0f) > eps)
                Assert::Fail(L"Line from point and normal vector computed incorrectly");

            {
                line2d<float> p1 = { -1.0f, 2.0f, -3.0f };
                line2d<float> p2 = { 2.0f, -4.0f, 6.0f };
                line2d<float> p3 = { -2.0f, -4.0f, 6.0f };

                Assert::IsTrue(p1.proportional(p2));
                Assert::IsFalse(p1.proportional(p3));
            }

            {
                line2d<float> p1 = { -1.0f, 2.0f, -3.0f };
                line2d<float> p2 = { 2.0f, -4.0f, -6.0f };
                line2d<float> p3 = { 2.0f, -4.0f, 6.0f };

                Assert::IsTrue(p1.parallel(p2));
                Assert::IsFalse(p1.parallel(p3));
            }

            {
                line2d<float> p1 = { -1.0f, 2.0f, -3.0f };
                line2d<float> p2 = { -2.0f, -4.0f, 6.0f };
                line2d<float> p3 = { 2.0f, -4.0f, 6.0f };

                Assert::IsTrue(p1.intersects(p2));
                Assert::IsFalse(p1.intersects(p3));
            }

            {
                vec2f point = { 1.0f, 0.0f };
                line2d<float> line = { 3.0f, 1.0f, 7.0f };
                vec2f dot = intersectionOfLineAndPoint(point, line);
                if (dot != vec2f(-2.0f, -1.0f))
                    Assert::Fail(L"intersectionOfLineAndPoint computed incorrectly");
            }
        }
    };
}
