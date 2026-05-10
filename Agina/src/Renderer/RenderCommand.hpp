#pragma once
#include <Renderer/Texture.hpp>
#include <glm/glm.hpp>

namespace Agina {

    struct RenderCommand
    {
        enum Type
        {
            Quad
        };

        Type type;

        glm::vec2 position;
        glm::vec2 size;

        Texture* texture;
    };

}