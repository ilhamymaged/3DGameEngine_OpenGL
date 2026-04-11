#pragma once

#include <Renderer/Shader.hpp>
#include <Renderer/Model.hpp>
#include <Renderer/Texture.hpp>
#include <iostream>
#include <unordered_map>

namespace Agina
{
    class AssetManager
    {
    public:
        static void LoadAsset(const std::string& name, const std::string& path);
        static void Clear();

    private:
        static bool IsTexture(const std::string& filePath);
		static bool IsModel(const std::string& filePath);
		static bool IsShader(const std::string& filePath);

		static Texture& LoadTexture(const std::string& name, const std::string& path);
		static Model& LoadModel(const std::string& name, const std::string& path);
		static Shader& LoadShader(const std::string& name, const std::string& path);

    private:
		static std::unordered_map<std::string, Texture> s_Textures;
        static std::unordered_map<std::string, Model> s_Models;
		static std::unordered_map<std::string, Shader> s_Shaders;
    };
}
