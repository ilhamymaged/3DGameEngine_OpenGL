#include <Renderer/Shader.hpp>    
#include <filesystem>
#include <Core/Logger.hpp>
#include <fstream>
#include <sstream> 

namespace Agina
{
    Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragPath)
        :m_Name(name), m_VertexPath(vertexPath), m_FragPath(fragPath)
    {
        CompileAndLink();
    };

    void Shader::setMat4(const std::string& name, const Mat4& mat)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, &mat.data[0]);
    }


    void Shader::setVec4(const std::string& name, const Vec4& vec)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniform4fv(loc, 1, &vec.x);
    }

    void Shader::setVec3(const std::string& name, const Vec3& vec)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniform3fv(loc, 1, &vec.x);
    }

    void Shader::setInt(const std::string& name, int value)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniform1i(loc, value);
    }

    void Shader::setFloat(const std::string& name, float value)
    {   
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniform1f(loc, value);
    }

    void Shader::SetBool(const std::string& name, bool value)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniform1i(loc, value);
    }

    int Shader::GetUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        int loc = glGetUniformLocation(m_programID, name.c_str());

        if (loc == -1)
        {
            AG_CORE_WARN("In " + m_Name + ": " + name + " is not used or found (might be optimized out by compiler).");
        }

        m_UniformLocationCache[name] = loc;
        return loc;
    }

    bool Shader::CompileAndLink()
    {
        std::ifstream vShaderFile(m_VertexPath);
        std::ifstream fShaderFile(m_FragPath);
        std::stringstream vShaderStream, fShaderStream;

        if (!vShaderFile.is_open())
        {
            AG_CORE_ERROR("Failed to open vertex shader: " + m_VertexPath);
            return false;
        }

        if (!fShaderFile.is_open())
        {
            AG_CORE_ERROR("Failed to open fragment shader: " + m_FragPath);
            return false;
        }

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        std::string vertexCode = vShaderStream.str();
        std::string fragmentCode = fShaderStream.str();
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            AG_CORE_ERROR(m_Name + ": ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
            AG_CORE_ERROR(infoLog);
            glDeleteShader(vertex);
            return false;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            AG_CORE_ERROR(m_Name + ": ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
            AG_CORE_ERROR(infoLog);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return false;
        }

        uint32_t newProgramID = glCreateProgram();
        glAttachShader(newProgramID, vertex);
        glAttachShader(newProgramID, fragment);
        glLinkProgram(newProgramID);

        glGetProgramiv(newProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(newProgramID, 512, NULL, infoLog);
            AG_CORE_ERROR(m_Name + ": ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
            AG_CORE_ERROR(infoLog);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(newProgramID);
            return false;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        if (m_programID != 0)
        {
            glDeleteProgram(m_programID);
        }

        m_programID = newProgramID;
        m_UniformLocationCache.clear();
        return true;
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_programID);
    }

    void Shader::Reload()
    {
        AG_CORE_INFO("Reloading shader: {0}", m_Name);

        if (CompileAndLink())
        {
            AG_CORE_INFO("Shader {0} successfully reloaded!", m_Name);
        }
        else
        {
            AG_CORE_ERROR("Failed to reload shader {0}. Keeping previous version.", m_Name);
        }
    }

    Shader& Shader::Use()
    {
        glUseProgram(m_programID);
        return *this;
    }
}