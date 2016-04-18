#pragma once

#include "Enity.h"

class Player : public Entity
{
public:
	virtual ~Player() { }
	virtual std::string GetName();
};