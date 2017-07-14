#pragma once

class IManager
{
public:
    IManager() = default;
    virtual ~IManager() noexcept = default;

    IManager(const IManager & ref) = delete;
    IManager(IManager && ref) = delete;
    IManager operator=(const IManager & ref) = delete;
    IManager operator=(IManager && ref) = delete;
};
