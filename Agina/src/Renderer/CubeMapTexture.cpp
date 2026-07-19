#include <glad/glad.h>
#include "CubeMapTexture.hpp"

#include <stb_image/stb_image.h>
#include <Core/Logger.hpp>

namespace Agina {

	CubemapTexture::CubemapTexture(const std::vector<std::string>& facePaths)
	{
		if (facePaths.size() != 6) 
		{
			AG_CORE_ERROR("Cubemap requires exactly 6 face paths, provided: {0}", facePaths.size());
			return;
		}

		stbi_set_flip_vertically_on_load(false);

        int width = 0, height = 0, nrChannels = 0;
        unsigned char* facesData[6] = { nullptr };
        GLenum internalFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;

        facesData[0] = stbi_load(facePaths[0].c_str(), &width, &height, &nrChannels, 0);
        if (!facesData[0]) 
        {
            AG_CORE_ERROR("Failed to load first cubemap face: {0}", facePaths[0]);
            return;
        }

        m_Width = static_cast<uint32_t>(width);
        m_Height = static_cast<uint32_t>(height);

        if (nrChannels == 1) { internalFormat = GL_R8; dataFormat = GL_RED; }
        else if (nrChannels == 3) { internalFormat = GL_RGB8; dataFormat = GL_RGB; }
        else if (nrChannels == 4) { internalFormat = GL_RGBA8; dataFormat = GL_RGBA; }

        for (int i = 1; i < 6; ++i) {
            int w, h, comp;
            facesData[i] = stbi_load(facePaths[i].c_str(), &w, &h, &comp, 0);
            if (!facesData[i] || w != width || h != height) {
                AG_CORE_ERROR("Cubemap face missing or mismatched dimensions: {0}", facePaths[i]);
                for (int j = 0; j <= i; ++j) { if (facesData[j]) stbi_image_free(facesData[j]); }
                return;
            }
        }

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

        for (int i = 0; i < 6; ++i) 
        {
            glTextureSubImage3D(m_RendererID, 0, 0, 0, i, m_Width, m_Height, 1, 
                dataFormat, GL_UNSIGNED_BYTE, facesData[i]);
            stbi_image_free(facesData[i]);
        }

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	CubemapTexture::~CubemapTexture()
	{
        if (m_RendererID) glDeleteTextures(1, &m_RendererID);
	}

    CubemapTexture::CubemapTexture(CubemapTexture&& other) noexcept
        : m_RendererID(other.m_RendererID), m_Width(other.m_Width), m_Height(other.m_Height) 
    {
        other.m_RendererID = 0;
        other.m_Width = 0;
        other.m_Height = 0;
    }

    CubemapTexture& CubemapTexture::operator=(CubemapTexture&& other) noexcept 
    {
        if (this != &other) {
            if (m_RendererID) glDeleteTextures(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            other.m_RendererID = 0;
            other.m_Width = 0;
            other.m_Height = 0;
        }
        return *this;
    }

	void CubemapTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}

