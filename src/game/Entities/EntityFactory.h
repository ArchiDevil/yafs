#pragma once

#include "Enity.h"

class EntityFactory
{
	EntityFactory() { }
public:
	enum EntityType
	{
		ET_Player
	};

	virtual ~EntityFactory() { }
	static Entity * GetEnity(EntityType type);
};