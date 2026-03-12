#pragma once

#include <filesystem>
#include <iostream>

static std::filesystem::path GetEngineRoot()
{
    namespace fs = std::filesystem;
    fs::path exePath = fs::current_path(); 

    fs::path current = exePath;
    while (!current.empty())
    {
        if (fs::exists(current / "Agina" / "assets")) return current / "Agina" 
            / "assets";
        current = current.parent_path();
    }

    throw std::runtime_error("Engine root not found!");
}
