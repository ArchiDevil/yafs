#pragma once

#include <string>

class Entity
{
public:
	virtual ~Entity() { }

	virtual std::string GetName() = 0;
	virtual void Show() { }
	virtual void Hide() { }
	virtual void Move(double x, double y) { }
};
