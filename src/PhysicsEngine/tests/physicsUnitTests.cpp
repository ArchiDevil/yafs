#include "CppUnitTest.h"

#include <PhysicsEngine/Manager.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PhysicsTest
{

TEST_CLASS(PhysicsTests)
{
public:
    bool AreEqualInEPS(float first, float second)
    {
        return std::fabs(first - second) < 0.0005f;
    }

    bool AreEqualInEPS(MathLib::vec2f first, MathLib::vec2f second)
    {
        return std::fabs(first.x - second.x) < 0.0005f
            && std::fabs(first.y - second.y) < 0.0005f;
    }

    TEST_METHOD(EntityTest)
    {
        Physics::PhysicsManager manager;
        MathLib::vec2f startPosition = { -12.0f, 56.5f };
        float size = 42.0f;
        MathLib::vec2f startSpeed = { 25.0f, 0.0f };

        std::shared_ptr<Physics::Entity> entity = manager.CreateEntity(startPosition, size, startSpeed);
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition(), startPosition));
        Assert::IsTrue(AreEqualInEPS(entity->GetSize(), size));
        Assert::IsTrue(AreEqualInEPS(entity->GetSpeed(), startSpeed));
        Assert::IsTrue(AreEqualInEPS(entity->GetElasticity(), 0.5f));

        // check speed updating
        entity->Update(1.0, { 0.0f, 0.0f });
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().x, startPosition.x + startSpeed.x));
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().y, startPosition.y));
        Assert::IsTrue(AreEqualInEPS(entity->GetSize(), size));
        Assert::IsTrue(AreEqualInEPS(entity->GetSpeed(), startSpeed));
        Assert::IsTrue(AreEqualInEPS(entity->GetElasticity(), 0.5f));

        // check acceleration updating
        entity->Update(1.0, { 1.0f, 0.0f });
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().x, startPosition.x + startSpeed.x + startSpeed.x + 1.0f));
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().y, startPosition.y));
        Assert::IsTrue(AreEqualInEPS(entity->GetSize(), size));
        Assert::IsTrue(AreEqualInEPS(entity->GetSpeed(), { startSpeed.x + 1.0f, startSpeed.y }));
        Assert::IsTrue(AreEqualInEPS(entity->GetElasticity(), 0.5f));
    }

    TEST_METHOD(EntityManagerTest)
    {
        Physics::PhysicsManager manager;
        auto entity = manager.CreateEntity({}, 0.0, {});
        Assert::IsFalse(entity.get() == nullptr);
    }

    TEST_METHOD(SolverTest)
    {
        Physics::PhysicsManager manager;

        // these two entities should collide
        auto entity1 = manager.CreateEntity({ 0.0, 0.0 }, 1.0, { 0.0, 0.0 });
        auto entity2 = manager.CreateEntity({ 1.5, 0.0 }, 2.0, { 0.0, 0.0 });

        manager.Update(1.0);

        Assert::IsTrue(entity1->GetPosition().x < 0.0f);
        Assert::IsTrue(AreEqualInEPS(entity1->GetPosition().y, 0.0f));
        Assert::IsTrue(entity1->GetSpeed().x < 0.0f);
        Assert::IsTrue(AreEqualInEPS(entity1->GetSpeed().y, 0.0f));

        Assert::IsTrue(entity2->GetPosition().x > 1.5f);
        Assert::IsTrue(AreEqualInEPS(entity2->GetPosition().y, 0.0f));
        Assert::IsTrue(entity2->GetSpeed().x > 0.0f);
        Assert::IsTrue(AreEqualInEPS(entity2->GetSpeed().y, 0.0f));

        // friction test - every entity should slow down without any other forces from outer space
        entity1 = manager.CreateEntity({ -10.0, 0.0 }, 1.0, { 0.0, 10.0 });

        float lastYSpeed = entity1->GetSpeed().y;
        for (size_t i = 0; i < 10; ++i)
        {
            manager.Update(0.1);

            Assert::IsTrue(AreEqualInEPS(entity1->GetPosition().x, -10.0f));
            Assert::IsTrue(entity1->GetPosition().y > 0.0f);

            Assert::IsTrue(AreEqualInEPS(entity1->GetSpeed().x, 0.0f));
            Assert::IsTrue(entity1->GetSpeed().y < lastYSpeed);
            lastYSpeed = entity1->GetSpeed().y;
        }
    }
};

}
