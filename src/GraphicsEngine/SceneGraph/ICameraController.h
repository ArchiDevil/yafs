#pragma once

#include <memory>

class ICameraController
{
public:
    virtual ~ICameraController()
    {
    }
};

using ICameraControllerPtr = std::unique_ptr<ICameraController>;
