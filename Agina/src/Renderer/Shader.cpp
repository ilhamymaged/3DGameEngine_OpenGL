#include <Renderer/Shader.hpp>    
#include <filesystem>
#include <Core/Logger/Logger.hpp>
#include <fstream>
#include <sstream> 

namespace Agina
{
    Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragPath)
        :m_Name(name)
    {
        std::ifstream vShaderFile(vertexPath);
        std::ifstream fShaderFile(fragPath);
        std::stringstream vShaderStream, fShaderStream;

        if (!vShaderFile.is_open())
            AG_CORE_ERROR("Failed to open vertex shader: " + vertexPath);

        if (!fShaderFile.is_open())
            AG_CORE_ERROR("Failed to open fragment shader: " + fragPath);

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

        // Check for shader compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            AG_CORE_ERROR(m_Name + ": ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
            AG_CORE_ERROR(infoLog);
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
        }

        m_programID = glCreateProgram();
        glAttachShader(m_programID, vertex);
        glAttachShader(m_programID, fragment);
        glLinkProgram(m_programID);

        glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
            AG_CORE_ERROR(m_Name + ": ERROR::SHADER::PROGRAM::LINKING_FAILED\n"); 
            AG_CORE_ERROR(infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    void Shader::setMat4(const std::string& name, const glm::mat4& mat)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, &(mat)[0][0]);
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& vec)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniform4fv(loc, 1, &vec[0]);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& vec)
    {
        int loc = GetUniformLocation(name);
        if (loc != -1) glUniform3fv(loc, 1, &vec[0]);
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

    Shader::~Shader()
    {
        glDeleteProgram(m_programID);
    }

    Shader& Shader::Use()
    {
        glUseProgram(m_programID);
        return *this;
    }
}