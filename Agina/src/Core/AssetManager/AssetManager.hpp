#pragma once

#include <Renderer/Shader.hpp>
#include <Renderer/Texture.hpp>
#include <iostream>
#include <unordered_map>

namespace Agina
{
    class AssetManager
    {
    public:
		static Texture& LoadTexture(const std::string& name, const std::string& path);
		static Shader& LoadShader(const std::string& name, const std::string& path);

        static Shader& GetShader(const std::string& name);
        static Texture& GetTexture(const std::string& name);

        static void Clear();
    private:
		static std::unordered_map<std::string, Texture> s_Textures;
		static std::unordered_map<std::string, Shader> s_Shaders;
    };
}
