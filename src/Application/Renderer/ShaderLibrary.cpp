#include <Application/Renderer/ShaderLibrary.hpp>

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::shaders;

void ShaderLibrary::Load(const std::string& name,
    const std::string& vertex,
    const std::string& fragment)
{
    shaders[name] = std::make_shared<Shader>(vertex.c_str(), fragment.c_str());
}

std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
{
    auto it = shaders.find(name);

    if (it == shaders.end())
        throw std::runtime_error("Shader not found: " + name);

    return it->second;
}