#pragma once

#include <string>
#include <vector>

namespace utils
{
    std::string narrow(const std::wstring & str);
    std::wstring widen(const std::string & str);

    std::string extract_extension(const std::string & filename);
    std::wstring extract_extension(const std::wstring & filename);

    std::string extract_name(const std::string & filename);
    std::wstring extract_name(const std::wstring & filename);

    bool is_number(const std::string & str);
    bool is_number(const std::wstring & str);

    namespace filesystem
    {
        bool create_dir(const std::string & directoryName);
        bool create_dir(const std::wstring & directoryName);

        bool is_directory(const std::string & directoryName);
        bool is_directory(const std::wstring & directoryName);

        bool delete_dir(const std::string & directoryName);
        bool delete_dir(const std::wstring & directoryName);

        bool remove_file(const std::string & fileName);
        bool remove_file(const std::wstring & fileName);

        std::vector<std::string> collect_file_names(const std::string & Path, const std::string & Extension = "*");
        std::vector<std::wstring> collect_file_names(const std::wstring & Path, const std::wstring & Extension = L"*");
    }
}
