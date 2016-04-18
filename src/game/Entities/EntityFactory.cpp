#include "EntityFactory.h"
#include "Player.h"

Entity * EntityFactory::GetEnity(EntityType type)
{
	Entity * entity = nullptr;
	switch (type)
	{
	case ET_Player:
		entity = new Player();
		break;
	default: { }
	}

	return entity;
}