#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

namespace Agina
{
    class Shader
    {
    public:
        Shader(const std::string& name, const std::string& vertexName, const std::string& fragName);
        ~Shader();

        Shader& Use();
        void setMat4(const std::string& name, const glm::mat4& mat);
        void setVec4(const std::string& name, const glm::vec4& vec);
        void setVec3(const std::string& name, const glm::vec3& vec);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);

    private:
        uint32_t m_programID;
        std::string m_Name;
    };
}