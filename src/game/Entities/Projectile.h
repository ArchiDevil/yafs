#pragma once

#include "Entity.h"

class Projectile : public Entity
{
public:
	virtual ~Projectile() { }
	std::string GetName() override;
};