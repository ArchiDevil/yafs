#include "ut.h"

#include "logger.hpp"

#if defined (WIN32) || (_WIN32)
    #include <Windows.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <codecvt> 

extern Log MainLog("Application.log");

std::string utils::narrow(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ucs2conv;
    return ucs2conv.to_bytes(str);
}

std::wstring utils::widen(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ucs2conv;
    return ucs2conv.from_bytes(str);
}

std::vector<std::string> utils::filesystem::collect_file_names(const std::string & Path, const std::string & Extension)
{
#if defined (WIN32) || (_WIN32)
    std::string bufferPath = Path;
    //check path here
    if (bufferPath[bufferPath.size() - 1] != '\\' && bufferPath[bufferPath.size() - 1] != '/')
        bufferPath.push_back('/');

    std::vector<std::string> names;
    WIN32_FIND_DATAA data;
    HANDLE file;
    std::string fName = std::string(bufferPath + "*." + Extension);
    file = FindFirstFileA(fName.c_str(), &data);
    if (file == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR("Unable to get filenames from ", bufferPath, ", may be directory is not exist or it's empty.");
        return std::vector<std::string>();
    }

    std::string buffer = data.cFileName;
    if (buffer != "." && buffer != "..")
    {
        names.push_back(buffer);
    }

    while (FindNextFileA(file, &data))
    {
        buffer = data.cFileName;
        if (buffer != "." && buffer != "..")
        {
            names.push_back(buffer);
        }
    }
    FindClose(file);
    return names;
#else
    LOG_ERROR("Not implemented yet.");
    return std::vector<std::string>();
#endif
}

std::vector<std::wstring> utils::filesystem::collect_file_names(const std::wstring & Path, const std::wstring & Extension /*= L"*"*/)
{
#if defined (WIN32) || (_WIN32)
    std::wstring bufferPath = Path;
    //check path here
    if (bufferPath[bufferPath.size() - 1] != L'\\' && bufferPath[bufferPath.size() - 1] != L'/')
        bufferPath.push_back(L'\\');

    std::vector<std::wstring> names;
    WIN32_FIND_DATAW data;
    HANDLE file;
    std::wstring fName = std::wstring(bufferPath + L"*." + Extension);
    file = FindFirstFileW(fName.c_str(), &data);
    if (file == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR("Unable to get filenames from ", utils::narrow(bufferPath), ", may be directory is not exist or it's empty.");
        return std::vector<std::wstring>();
    }

    std::wstring buffer = data.cFileName;
    if (buffer != L"." && buffer != L"..")
    {
        names.push_back(buffer);
    }

    while (FindNextFileW(file, &data))
    {
        buffer = data.cFileName;
        if (buffer != L"." && buffer != L"..")
        {
            names.push_back(buffer);
        }
    }
    FindClose(file);
    return names;
#else
    LOG_ERROR("Not implemented yet.");
    return std::vector<std::wstring>();
#endif
}

std::string utils::extract_name(const std::string & filename)
{
    std::string::size_type pos = filename.rfind('.');
    if (pos == filename.npos)
        return filename;
    else
        return filename.substr(0, pos);
}

std::wstring utils::extract_name(const std::wstring & filename)
{
    std::wstring::size_type pos = filename.rfind(L'.');
    if (pos == filename.npos)
        return filename;
    else
        return filename.substr(0, pos);
}

std::string utils::extract_extension(const std::string & filename)
{
    std::string::size_type pos = filename.rfind('.');
    if (pos == filename.npos)
        return std::string();
    else
        return filename.substr(pos + 1, filename.npos);
}

std::wstring utils::extract_extension(const std::wstring & filename)
{
    std::wstring::size_type pos = filename.rfind(L'.');
    if (pos == filename.npos)
        return std::wstring();
    else
        return filename.substr(pos + 1, filename.npos);
}

bool utils::is_number(const std::string & str)
{
    if (str.empty())
        return false;
    return std::all_of(str.cbegin(), str.cend(), ::isdigit);
}

bool utils::is_number(const std::wstring & str)
{
    if (str.empty())
        return false;
    return std::all_of(str.cbegin(), str.cend(), ::isdigit);
}

bool utils::filesystem::create_dir(const std::string & directoryName)
{
#ifdef WIN32
    return ::CreateDirectoryA(directoryName.c_str(), NULL) != 0;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::create_dir(const std::wstring & directoryName)
{
#ifdef WIN32
    return ::CreateDirectoryW(directoryName.c_str(), NULL) != 0;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::is_directory(const std::string & directoryName)
{
#ifdef WIN32
    DWORD ftyp = GetFileAttributesA(directoryName.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::is_directory(const std::wstring & directoryName)
{
#ifdef WIN32
    DWORD ftyp = GetFileAttributesW(directoryName.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::remove_file(const std::string & fileName)
{
#if defined (WIN32) || (_WIN32)
    return !!DeleteFileA(fileName.c_str());
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::remove_file(const std::wstring & fileName)
{
#if defined (WIN32) || (_WIN32)
    return !!DeleteFileW(fileName.c_str());
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}
