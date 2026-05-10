#pragma once

#include <iostream>
#include <filesystem>

namespace Agina {

    class FileSystem
    {
    public:
        static void Init(const std::filesystem::path& root);
        static std::filesystem::path GetAssetPath(const std::string& relative);

    private:
        static std::filesystem::path s_AssetRoot;
    };
}