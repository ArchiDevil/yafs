#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	virtual ~Player() { }
	std::string GetName() override;
};