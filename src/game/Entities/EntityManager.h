#pragma once

#include "Entity.h"
#include "Utilities/singleton.h"

#include <list>
#include <memory>

class EntityManager : public singleton <EntityManager>
{
public:
	EntityManager() {}
	virtual ~EntityManager() {}

	void AddEnity(std::shared_ptr<Entity> ent);
	void RemoveEntity(std::shared_ptr<Entity> ent);

	void UpdateAllEntities(double deltaTime);

private:
	std::list<std::shared_ptr<Entity>> entities;
};