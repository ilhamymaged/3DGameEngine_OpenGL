#pragma once

#include <Renderer/Mesh.hpp>
#include <Renderer/Model.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/Texture2D.hpp>
#include <Renderer/CubemapTexture.hpp>
#include <iostream>
#include <unordered_map>
#include <Agina.h>

namespace Agina
{
    class AssetManager
    {
    public:
		static std::Ref<Texture2D> Load2DTexture(const std::string& name, const std::string& path);
        static std::Ref<CubemapTexture> LoadCubemap(const std::string& name, const std::vector<std::string>& facePaths);
		static std::Ref<Shader> LoadShader(const std::string& name, const std::string& path);
        static std::Ref<Model> LoadModel(const std::string& name, const std::string& path);
        static std::Ref<Mesh> LoadMesh(MeshType type);

        static std::Ref<Mesh> GetMesh(MeshType type);
        static std::Ref<Model> GetModel(const std::string& name);
        static std::Ref<CubemapTexture> GetCubemap(const std::string& name); 
        static std::Ref<Shader> GetShader(const std::string& name);
        static std::Ref<Texture2D> Get2DTexture(const std::string& name);

        static void Clear();
    private:
		static std::unordered_map<std::string, std::Ref<Texture2D>> s_2DTextures;
        static std::unordered_map<std::string, std::Ref<CubemapTexture>> s_Cubemaps; 
        static std::unordered_map<std::string, std::Ref<Shader>> s_Shaders;
        static std::unordered_map<MeshType, std::Ref<Mesh>> s_Meshes;
        static std::unordered_map<std::string, std::Ref<Model>> s_Models;
    };
}
