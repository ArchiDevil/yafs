#include "iniloader.h"

#include "ut.h"
#include "logger.hpp"

bool IniWorker::Initialize(const std::string & FileName)
{
    std::ifstream File;
    File.open(FileName.c_str());
    if (File.fail())
    {
        LOG_FATAL_ERROR("Unable to open settings file");
        return false;
    }
    CollectKeys(File);
    return true;
}

std::string IniWorker::GetKey(const std::string & Key) const
{
    if (data.find(Key) != data.end())
        return data.at(Key);

    LOG_ERROR("Unable to get key for: ", Key);
    return "";
}

int IniWorker::GetInteger(const std::string & Parameter) const
{
    return std::stoi(GetKey(Parameter));
}

float IniWorker::GetFloat(const std::string & Parameter) const
{
    return std::stof(GetKey(Parameter));
}

bool IniWorker::GetBoolean(const std::string & Parameter) const
{
    std::string buffer = GetKey(Parameter);
    if (buffer == "true")
        return true;
    else if (buffer == "false")
        return false;
    else
    {
        LOG_ERROR(buffer, " is a wrong boolean value");
        return false;
    }
}

std::string IniWorker::GetString(const std::string & Parameter) const
{
    return GetKey(Parameter);
}

std::wstring IniWorker::GetWString(const std::string & Parameter) const
{
    return utils::Widen(GetKey(Parameter));
}

void IniWorker::CollectKeys(std::ifstream & stream)
{
    std::string buffer = "";
    while (std::getline(stream, buffer))
    {
        std::string::size_type it = buffer.find('=');
        if (it != std::string::npos)
        {
            std::string key = std::string(buffer.begin(), buffer.begin() + it);
            data[key] = std::string(buffer.begin() + it + 1, buffer.end());
        }
    }
}
