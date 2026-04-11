#include "AssetManager.hpp"
#include <Core/Logger/Logger.hpp>
#include <filesystem>

namespace Agina
{
    std::unordered_map<std::string, Model> AssetManager::s_Models;
    std::unordered_map<std::string, Texture> AssetManager::s_Textures;

    void AssetManager::LoadAsset(const std::string& name, const std::string& path)
    {
		if(IsShader(path)) LoadShader(name, path);
        else if(IsTexture(path)) LoadTexture(name, path);
        else if(IsModel(path)) LoadModel(name, path);
		else AG_CORE_ERROR("Unsupported asset type: {0}", path);
    }

    void AssetManager::Clear()
    {
        s_Models.clear();
        s_Textures.clear();

    }

    bool AssetManager::IsTexture(const std::string& filePath)
    {
        static const std::unordered_set<std::string> textureExts = { ".png", ".jpg", ".jpeg",
            ".bmp", ".tga", ".dds" };

		std::string ext = std::filesystem::path(filePath).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return textureExts.find(ext) != textureExts.end();
    }

    bool AssetManager::IsModel(const std::string& filePath)
    {
        static const std::unordered_set<std::string> modelExts = { ".obj", ".fbx", ".gltf", ".glb", ".dae" };
        std::string ext = std::filesystem::path(filePath).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return modelExts.find(ext) != modelExts.end();
    }

    bool AssetManager::IsShader(const std::string& filePath)
    {
        static const std::unordered_set<std::string> shaderExts = { ".vert", ".frag", ".geom", ".comp" ,
        ".vs", ".fr", ".fragment", ".vertex", ".glsl"};
        std::string ext = std::filesystem::path(filePath).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		return shaderExts.find(ext) != shaderExts.end();
    }

    Texture& AssetManager::LoadTexture(const std::string& name, const std::string& path)
    {
        auto it = s_Textures.find(path);

        if (it != s_Textures.end())
            return it->second;

        auto texture = Texture(path);
        s_Textures[path] = texture;
        AG_CORE_INFO("Loaded Texture {}", path);
        return s_Textures[path];

    }

    Model& AssetManager::LoadModel(const std::string& name, const std::string& path)
    {
        auto it = s_Models.find(path);
        if (it != s_Models.end())
            return it->second;

        auto model = Model(path);
        s_Models[path] = model;
        AG_CORE_INFO("Loaded Model {}", path);
		return s_Models[path];
    }

    Shader& AssetManager::LoadShader(const std::string& name, const std::string& path)
    {
        auto it = s_Shaders.find(path);
        if (it != s_Shaders.end())
            return it->second;

        std::string vertexPath = path + ".vert";
        std::string fragPath = path + ".frag";
        auto shader = Shader(name, vertexPath, fragPath);
        s_Shaders[path] = shader;
		AG_CORE_INFO("Loaded Shader {}", path);
        return s_Shaders[path];
    }

}