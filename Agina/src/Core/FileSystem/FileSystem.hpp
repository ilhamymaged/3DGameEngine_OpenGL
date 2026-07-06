#pragma once

#include <iostream>
#include <filesystem>

namespace Agina {

    class FileSystem
    {
    public:
        static void Init(const std::filesystem::path& root);
        static std::filesystem::path EngineAssets();

    private:
        static std::filesystem::path s_AssetRoot;
    };
}