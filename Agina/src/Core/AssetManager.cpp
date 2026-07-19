#include "AssetManager.hpp"
#include <Core/Logger.hpp>
#include <filesystem>

namespace Agina
{
    std::unordered_map<std::string, std::Ref<Texture2D>> AssetManager::s_2DTextures;
    std::unordered_map<std::string, std::Ref<CubemapTexture>> AssetManager::s_Cubemaps;
	std::unordered_map<std::string, std::Ref<Shader>> AssetManager::s_Shaders;
    std::unordered_map<MeshType, std::Ref<Mesh>> AssetManager::s_Meshes;
    std::unordered_map<std::string, std::Ref<Model>> AssetManager::s_Models;

    void AssetManager::Clear()
    {
        s_2DTextures.clear();
        s_Cubemaps.clear();
        s_Shaders.clear();
        s_Meshes.clear();
        s_Models.clear();
    }

    std::Ref<CubemapTexture> AssetManager::LoadCubemap(const std::string& name, const std::vector<std::string>& facePaths)
    {
        auto it = s_Cubemaps.find(name);
        if (it != s_Cubemaps.end())
            return it->second;

        auto cubemap = std::make_Ref<CubemapTexture>(facePaths);
        s_Cubemaps[name] = cubemap;
        return cubemap;
    }

    std::Ref<CubemapTexture> AssetManager::GetCubemap(const std::string& name)
    {
        auto it = s_Cubemaps.find(name);
        if (it != s_Cubemaps.end())
            return it->second;

        AG_CORE_ERROR("Didn't Find Cubemap With Name {0}", name);
        throw std::runtime_error("Didn't Find Cubemap With Name: " + name);
    }

    std::Ref<Texture2D> AssetManager::Load2DTexture(const std::string& name, const std::string& path)
    {
        auto it = s_2DTextures.find(name);

        if (it != s_2DTextures.end())
            return it->second;

        s_2DTextures.emplace(name, std::make_Ref<Texture2D>(path));
        return s_2DTextures.find(name)->second;
    }

    std::Ref<Texture2D> AssetManager::Get2DTexture(const std::string& name)
    {
        auto it = s_2DTextures.find(name);
        if (it != s_2DTextures.end())
            return it->second;
        else
        {
            AG_CORE_ERROR("Didn't Find Texture With Name {0}", name);
            throw std::runtime_error("Didn't Find Texture With Name: " + name);
        }
    }

    std::Ref<Shader> AssetManager::LoadShader(const std::string& name, const std::string& path)
    {
        auto it = s_Shaders.find(name);
        if (it != s_Shaders.end())
            return it->second;

        std::string vertexPath = path + ".vert";
        std::string fragPath = path + ".frag";

        auto shader = std::make_Ref<Shader>(name, vertexPath, fragPath);
        s_Shaders[name] = shader;
        
        return shader;
    }

    std::Ref<Shader> AssetManager::GetShader(const std::string& name)
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

    std::Ref<Model> AssetManager::LoadModel(const std::string& name, const std::string& path)
    {
        auto it = s_Models.find(name);
        if (it != s_Models.end())
            return it->second;

        s_Models.emplace(name, std::make_Ref<Model>(path));
        return s_Models.find(name)->second;
    }

    std::Ref<Model> AssetManager::GetModel(const std::string& name)
    {
        auto it = s_Models.find(name);
        if (it != s_Models.end())
            return it->second;
        else
        {
            AG_CORE_ERROR("Didn't Find A Model With Name {0}", name);
            throw std::runtime_error("Didn't Find A Model With Name: " + name);
        }
    }
    
    std::Ref<Mesh> AssetManager::LoadMesh(MeshType type)
    {
        auto it = s_Meshes.find(type);
        if (it != s_Meshes.end())
            return it->second;

        s_Meshes.emplace(type, Mesh::Create(type));
        return s_Meshes.find(type)->second;
    }

    std::Ref<Mesh> AssetManager::GetMesh(MeshType type)
    {
        auto it = s_Meshes.find(type);
        if (it != s_Meshes.end())
            return it->second;
        else
        {
            AG_CORE_ERROR("Didn't Find A Mesh With Type");
            throw std::runtime_error("Didn't Find This Mesh"); 
        }
    }
 
}