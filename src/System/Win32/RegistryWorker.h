#pragma once

#include <string>

//from windows.h
enum Root
{
    HKEY_CLASSES_ROOT_R = 0x80000000,
    HKEY_CURRENT_USER_R = 0x80000001,
    HKEY_LOCAL_MACHINE_R = 0x80000002,
    HKEY_USERS_R = 0x80000003,
    HKEY_CURRENT_CONFIG_R = 0x80000005,
};

class RegistryWorker
{
public:
    static std::wstring GetString(Root root, const std::wstring & path, const std::wstring & parameter);
    static int GetInteger(Root root, const std::wstring & path, const std::wstring & parameter);

    void SetString(Root root, const std::wstring & path, const std::wstring & param, const std::wstring & val);
    void SetInteger(Root rootm, const std::wstring & path, const std::wstring & param, int val);
};
