#pragma once

#include <Renderer/Mesh.hpp>
#include <Renderer/Model.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/Texture.hpp>
#include <iostream>
#include <unordered_map>

namespace Agina
{
    class AssetManager
    {
    public:
		static std::shared_ptr<Texture2D> Load2DTexture(const std::string& name, const std::string& path);
		static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& path);

        static std::shared_ptr<Model> LoadModel(const std::string& name, const std::string& path);
        static std::shared_ptr<Mesh> LoadMesh(MeshType type);

        static std::shared_ptr<Mesh> GetMesh(MeshType type);
        static std::shared_ptr<Model> GetModel(const std::string& name);

        static std::shared_ptr<Shader> GetShader(const std::string& name);
        static std::shared_ptr<Texture2D> Get2DTexture(const std::string& name);

        static void Clear();
    private:
		static std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_2DTextures;
		static std::unordered_map<std::string, std::shared_ptr<Shader>> s_Shaders;
        static std::unordered_map<MeshType, std::shared_ptr<Mesh>> s_Meshes;
        static std::unordered_map<std::string, std::shared_ptr<Model>> s_Models;
    };
}
