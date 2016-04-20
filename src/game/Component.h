#pragma once

#include <MathLib/math.h>

class IComponent
{
public:
    virtual void Update(double dt) = 0;
};

class TransformComponent : public IComponent
{
public:
    TransformComponent(const MathLib::vec3f & position, const MathLib::vec3f & rotation, const MathLib::vec3f & scale)
        : position(position)
        , rotation(rotation)
        , scale(scale)
    {}

    void Update(double dt) override
    {}

    void SetPosition(const MathLib::vec3f & position)
    {
        this->position = position;
    }

    void SetRotation(const MathLib::vec3f & rotation)
    {
        this->rotation = rotation;
    }

    void SetScale(const MathLib::vec3f & scale)
    {
        this->scale = scale;
    }

private:
    MathLib::vec3f position;
    MathLib::vec3f rotation;
    MathLib::vec3f scale;
};

class ScriptComponent : public IComponent
{
public:
    ScriptComponent(void * scriptHandle)
        : scriptHandle(scriptHandle)
    {}

    void Update(double dt) override
    {
        // update something in script
    }

private:
    void * scriptHandle = nullptr;
};

class RenderComponent : public IComponent
{
public:
    RenderComponent()
    {}

    void Update(double dt) override
    {
        // update animations and etc.
    }

    void Draw()
    {
        // draw object
    }
};

// and more components
