#include "PhysicsEntity.h"

PhysicsEntity::PhysicsEntity(const MathLib::vec2f & position,
                             ShiftEngine::SpriteSceneNode * sprite,
                             const std::shared_ptr<Physics::Entity>& physicsEntity)
    : Entity(position, sprite)
    , Physics::IPhysicsEntityHolder(physicsEntity)
{
}

void PhysicsEntity::Update(double dt)
{
    Entity::SetPosition(IPhysicsEntityHolder::physicsEntity->GetPosition());
}

MathLib::vec2f PhysicsEntity::GetPosition() const
{
    return IPhysicsEntityHolder::physicsEntity->GetPosition();
}

void PhysicsEntity::SetPosition(MathLib::vec2f pos)
{
    IPhysicsEntityHolder::physicsEntity->SetPosition(pos);
    Entity::SetPosition(IPhysicsEntityHolder::physicsEntity->GetPosition());
}
