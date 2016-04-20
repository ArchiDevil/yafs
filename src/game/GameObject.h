#pragma once

#include "Component.h"

#include <initializer_list>
#include <memory>
#include <vector>

class GameObject
{
public:
    GameObject(std::initializer_list<std::unique_ptr<IComponent>> components)
    {
        for (auto & component : components)
            this->components.push_back(std::move(component));
    }

    void Update(double dt)
    {
        for (auto & component : components)
            component->Update(dt);
    }

    template<typename T>
    T * GetComponent()
    {
        for (IComponent* component : components)
        {
            // may fail if components use inheritance one from another
            T* casted = dynamic_cast<T*>(component);
            if (casted)
                return casted;
        }
    }

private:
    std::vector<std::unique_ptr<IComponent>> components;
};
