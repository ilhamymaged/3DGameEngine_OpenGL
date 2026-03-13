#pragma once

#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Shader.hpp"

namespace Agina
{
    class ShaderLibrary
    {
    public:
        static void Load(const std::string& name,
            const std::string& vertexPath,
            const std::string& fragmentPath);

        static std::shared_ptr<Shader> Get(const std::string& name);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    };
}