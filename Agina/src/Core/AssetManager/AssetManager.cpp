#include "AssetManager.hpp"
#include <Core/Logger/Logger.hpp>

namespace Agina
{
    std::unordered_map<std::string, Model> AssetManager::s_Models;

    void AssetManager::Clear()
    {
        s_Models.clear();
    }

    Model &AssetManager::LoadModel(const std::string &name)
    {
        auto it = s_Models.find(name);
        if (it == s_Models.end())
            it = s_Models.insert(std::make_pair(name, Model(name))).first;
        AG_CORE_INFO("Model Loaded {}", name);
        return it->second;
    }
}
