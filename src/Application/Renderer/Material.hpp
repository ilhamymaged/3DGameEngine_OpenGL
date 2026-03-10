#pragma once

#include <Application/Renderer/Shader.hpp>
#include <Application/Renderer/Texture.hpp>
#include <iostream>

class Material
{
public:
    Material(std::shared_ptr<Shader> shader,
        std::vector<std::shared_ptr<Texture>> textures);

    void Bind();
    inline Shader& GetShader() { return *m_Shader; }
private:
    std::shared_ptr<Shader> m_Shader;
    std::vector<std::shared_ptr<Texture>> m_Textures;
};