#include <Application/Renderer/Shader.hpp>    
#include <filesystem>
#include <Application/Utility/LocU.hpp>

Shader::Shader(const std::string& vertexName, const std::string& fragName)
{
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::stringstream vShaderStream, fShaderStream;

    vShaderFile.open(GetEngineRoot() / "shaders" / vertexName);
    fShaderFile.open(GetEngineRoot() / "shaders" / fragName);
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    std::string vertexCode = vShaderStream.str();
    std::string fragmentCode = fShaderStream.str();
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Check for shader compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertex);
    glAttachShader(m_programID, fragment);
    glLinkProgram(m_programID);

    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
};

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
    uint32_t loc = glGetUniformLocation(m_programID, name.c_str());
    if(loc == -1) std::cerr << name << " is not used or found.\n";
    glUniformMatrix4fv(loc, 1, GL_FALSE, &(mat)[0][0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec)
{
    uint32_t loc = glGetUniformLocation(m_programID, name.c_str());
    if (loc == -1) std::cerr << name << " is not used or found.\n";
    glUniformMatrix3fv(loc, 1, GL_FALSE, &(vec)[0]);
}

void Shader::setInt(const std::string& name, int value)
{
    uint32_t loc = glGetUniformLocation(m_programID, name.c_str());
    if (loc == -1) std::cerr << name << " is not used or found.\n";
    glUniform1i(loc, value);
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

void Shader::Use() const
{
    glUseProgram(m_programID);
}
