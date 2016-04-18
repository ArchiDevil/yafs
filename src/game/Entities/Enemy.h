#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	virtual ~Enemy() { }
	std::string GetName() override;
};