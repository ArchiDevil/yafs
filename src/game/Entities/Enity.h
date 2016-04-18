#pragma once

#include <xstring>

class Entity
{
public:
	virtual ~Entity() { }
	virtual std::string GetName() = 0;
};