#pragma once

#include <glm/glm.hpp>

namespace Agina
{
    class Grid
    {
    public:
        Grid();
        ~Grid();

        void Draw(const glm::mat4& view, const glm::mat4& projection,
            const glm::vec3& cameraPos);
    private:
        unsigned int m_VAO;
        unsigned int m_VBO;
    };
}