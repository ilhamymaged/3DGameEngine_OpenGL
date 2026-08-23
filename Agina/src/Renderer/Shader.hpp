#pragma once

#include <glad/glad.h>
#include <string>
#include <Core/MathTypes.hpp>
#include <unordered_map>
#include <cstdint> 

namespace Agina
{
    class Shader
    {
    public:
        Shader(const std::string& name, const std::string& vertexName, const std::string& fragName);
        ~Shader();

        void Reload();

        Shader& Use();
        void setMat4(const std::string& name, const Mat4& mat); 
        void setVec4(const std::string& name, const Vec4& vec); 
        void setVec3(const std::string& name, const Vec3& vec);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void SetBool(const std::string& name, bool value);

    private:
        int GetUniformLocation(const std::string& name);
        bool CompileAndLink();

    private:
        uint32_t m_programID;
        std::string m_Name;
        std::string m_VertexPath;
        std::string m_FragPath; 
        std::unordered_map<std::string, int> m_UniformLocationCache;
    };
}
