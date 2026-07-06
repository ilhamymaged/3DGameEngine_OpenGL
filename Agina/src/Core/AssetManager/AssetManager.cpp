#include "AssetManager.hpp"
#include <Core/Logger/Logger.hpp>
#include <filesystem>

namespace Agina
{
    std::unordered_map<std::string, Texture> AssetManager::s_Textures;
	std::unordered_map<std::string, Shader> AssetManager::s_Shaders;

    void AssetManager::Clear()
    {
        s_Textures.clear();
        s_Shaders.clear();
    }

    Texture& AssetManager::LoadTexture(const std::string& name, const std::string& path)
    {
        auto it = s_Textures.find(name);

        if (it != s_Textures.end())
            return it->second;

		s_Textures.emplace(name, Texture(path));
        return s_Textures.find(name)->second;

    }

    Shader& AssetManager::LoadShader(const std::string& name, const std::string& path)
    {
        auto it = s_Shaders.find(name);
        if (it != s_Shaders.end())
            return it->second;

        std::string vertexPath = path + ".vert";
        std::string fragPath = path + ".frag";

        s_Shaders.emplace(name, Shader(name, vertexPath, fragPath));
        return s_Shaders.find(name)->second;
    }

    Shader& AssetManager::GetShader(const std::string& name)
    {
        auto it = s_Shaders.find(name);
        if (it != s_Shaders.end())
            return it->second;
        else
        {
            AG_CORE_ERROR("Didn't Find Shader With Name {0}", name);
            throw std::runtime_error("Didn't Find Shader With Name: " + name);
        }
    }

    Texture& AssetManager::GetTexture(const std::string& name)
    {
        auto it = s_Textures.find(name);
        if (it != s_Textures.end())
            return it->second;
        else
        {
            AG_CORE_ERROR("Didn't Find Texture With Name {0}", name);
            throw std::runtime_error("Didn't Find Texture With Name: " + name);
        }
    }

}