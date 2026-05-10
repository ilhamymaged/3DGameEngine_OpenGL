#include <Renderer/Shader.hpp>    
#include <filesystem>
#include <Core/Logger/Logger.hpp>

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
            AG_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
            AG_CORE_ERROR(infoLog);
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            AG_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
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
            AG_CORE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n"); 
            AG_CORE_ERROR(infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    void Shader::setMat4(const std::string& name, const glm::mat4& mat)
    {
        int loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) AG_CORE_ERROR("In " + m_Name + ": " + name + " is not used or found.\n");
        glUniformMatrix4fv(loc, 1, GL_FALSE, &(mat)[0][0]);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& vec)
    {
        int loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) AG_CORE_ERROR("In " + m_Name + ": " + name + " is not used or found.\n");
        glUniform3fv(loc, 1, &vec[0]);
    }

    void Shader::setInt(const std::string& name, int value)
    {
        int loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) AG_CORE_ERROR("In " + m_Name + ": " + name + " is not used or found.\n");
        glUniform1i(loc, value);
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

    Shader::Shader(Shader&& other) noexcept
    {
        m_programID = other.m_programID;
        m_Name = std::move(other.m_Name);
        other.m_programID = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept
    {
        if (this != &other)
        {
            glDeleteProgram(m_programID);
            m_programID = other.m_programID;
            m_Name = std::move(other.m_Name);
            other.m_programID = 0;
        }
        return *this;
    }
}