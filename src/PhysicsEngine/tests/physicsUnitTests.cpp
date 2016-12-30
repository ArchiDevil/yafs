#include "CppUnitTest.h"

#include <PhysicsEngine/PhysicsEngine.h>

#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PhysicsTest
{

TEST_CLASS(PhysicsTests)
{
    class TestHolder : public Physics::IPhysicsEntityHolder
    {
        std::function<void(Physics::IPhysicsEntityHolder*)> collisionHandler;

    public:
        TestHolder(const std::shared_ptr<Physics::Entity>& entity)
            : Physics::IPhysicsEntityHolder(entity)
        {
        }

        void OnCollision(Physics::IPhysicsEntityHolder* holder) override
        {
            if (collisionHandler)
                collisionHandler(holder);
        }

        void SetCollisionHandler(const std::function<void(Physics::IPhysicsEntityHolder*)> & handler)
        {
            this->collisionHandler = handler;
        }

        Physics::Entity * GetEntity() const
        {
            return physicsEntity.get();
        }
    };

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

        TestHolder holder{ manager.CreateEntity(startPosition, size, startSpeed) };
        Physics::Entity * entity = holder.GetEntity();
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition(), startPosition));
        Assert::IsTrue(AreEqualInEPS(entity->GetSize(), size));
        Assert::IsTrue(AreEqualInEPS(entity->GetVelocity(), startSpeed));
        Assert::IsTrue(AreEqualInEPS(entity->GetElasticity(), 0.5f));

        // check speed updating
        entity->Update(1.0, { 0.0f, 0.0f });
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().x, startPosition.x + startSpeed.x));
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().y, startPosition.y));
        Assert::IsTrue(AreEqualInEPS(entity->GetSize(), size));
        Assert::IsTrue(AreEqualInEPS(entity->GetVelocity(), startSpeed));
        Assert::IsTrue(AreEqualInEPS(entity->GetElasticity(), 0.5f));

        // check acceleration updating
        entity->Update(1.0, { 1.0f, 0.0f });
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().x, startPosition.x + startSpeed.x + startSpeed.x + 1.0f));
        Assert::IsTrue(AreEqualInEPS(entity->GetPosition().y, startPosition.y));
        Assert::IsTrue(AreEqualInEPS(entity->GetSize(), size));
        Assert::IsTrue(AreEqualInEPS(entity->GetVelocity(), { startSpeed.x + 1.0f, startSpeed.y }));
        Assert::IsTrue(AreEqualInEPS(entity->GetElasticity(), 0.5f));
    }

    TEST_METHOD(EntityManagerTest)
    {
        Physics::PhysicsManager manager;
        TestHolder holder{ manager.CreateEntity({}, 0.0, {}) };
        Assert::IsFalse(nullptr == holder.GetEntity());
    }

    TEST_METHOD(SolverTest)
    {
        Physics::PhysicsManager manager;

        // these two entities should collide
        TestHolder holder1{ manager.CreateEntity({ 0.0, 0.0 }, 1.0,{ 0.0, 0.0 }) };
        TestHolder holder2{ manager.CreateEntity({ 1.5, 0.0 }, 2.0,{ 0.0, 0.0 }) };

        Physics::Entity * entity1 = holder1.GetEntity();
        Physics::Entity * entity2 = holder2.GetEntity();

        manager.Update(1.0);

        Assert::IsTrue(entity1->GetPosition().x < 0.0f);
        Assert::IsTrue(AreEqualInEPS(entity1->GetPosition().y, 0.0f));
        Assert::IsTrue(entity1->GetVelocity().x < 0.0f);
        Assert::IsTrue(AreEqualInEPS(entity1->GetVelocity().y, 0.0f));

        Assert::IsTrue(entity2->GetPosition().x > 1.5f);
        Assert::IsTrue(AreEqualInEPS(entity2->GetPosition().y, 0.0f));
        Assert::IsTrue(entity2->GetVelocity().x > 0.0f);
        Assert::IsTrue(AreEqualInEPS(entity2->GetVelocity().y, 0.0f));

        // friction test - every entity should slow down without any other forces from outer space
        holder1 = { manager.CreateEntity({ -10.0, 0.0 }, 1.0, { 0.0, 10.0 }) };
        entity1 = holder1.GetEntity();

        float lastYSpeed = entity1->GetVelocity().y;
        for (size_t i = 0; i < 10; ++i)
        {
            manager.Update(0.1);

            Assert::IsTrue(AreEqualInEPS(entity1->GetPosition().x, -10.0f));
            Assert::IsTrue(entity1->GetPosition().y > 0.0f);

            Assert::IsTrue(AreEqualInEPS(entity1->GetVelocity().x, 0.0f));
            Assert::IsTrue(entity1->GetVelocity().y < lastYSpeed);
            lastYSpeed = entity1->GetVelocity().y;
        }
    }

    TEST_METHOD(CollisionTest)
    {
        Physics::PhysicsManager manager;

        // these two entities should collide
        TestHolder holder1{ manager.CreateEntity({ 0.0, 0.0 }, 1.0,{ 0.0, 0.0 }) };
        TestHolder holder2{ manager.CreateEntity({ 1.5, 0.0 }, 2.0,{ 0.0, 0.0 }) };
        bool collision1 = false;
        bool collision2 = false;
        holder1.SetCollisionHandler([&](Physics::IPhysicsEntityHolder* holder)
        {
            Assert::IsTrue((Physics::IPhysicsEntityHolder*)&holder2 == holder);
            collision1 = true;
        }
        );
        holder2.SetCollisionHandler([&](Physics::IPhysicsEntityHolder* holder)
        {
            Assert::IsTrue((Physics::IPhysicsEntityHolder*)&holder1 == holder);
            collision2 = true;
        }
        );

        manager.Update(1.0);
        Assert::AreEqual(true, collision1);
        Assert::AreEqual(true, collision2);
    }
};

}
