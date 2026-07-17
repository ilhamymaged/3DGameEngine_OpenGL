#include "Texture.hpp"
#include <Core/Logger.hpp>
#include <stb_image/stb_image.h>

namespace Agina
{
    Texture2D::Texture2D(const std::string& path) : m_Path(path)
    {
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum internalFormat = 0, dataFormat = 0;
            if (nrChannels == 1) { internalFormat = GL_R8;    dataFormat = GL_RED; }
            if (nrChannels == 3) { internalFormat = GL_RGB8;  dataFormat = GL_RGB; }
            if (nrChannels == 4) { internalFormat = GL_RGBA8; dataFormat = GL_RGBA; }

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
			AG_CORE_ERROR("Failed To Load Texture: {0}", path);
        stbi_image_free(data);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_ID);
    }

    void Texture2D::Bind(uint32_t slot) 
    {
        glActiveTexture(GL_TEXTURE0 + slot); 
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }
}
