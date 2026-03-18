#pragma once

#include <iostream>
#include <unordered_map>
#include <Renderer/Model.hpp>

namespace Agina
{
    class AssetManager
    {
    public:
        static void Clear();
        static Model& LoadModel(const std::string& name);

    private:
        static std::unordered_map<std::string, Model> s_Models;
    };
}
