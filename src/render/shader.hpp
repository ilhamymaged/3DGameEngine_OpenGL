#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>  
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void Use() const;
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec3(const std::string& name, const glm::vec3& vec);
    void setInt(const std::string& name, int value);

private:
    unsigned int m_programID;
};
