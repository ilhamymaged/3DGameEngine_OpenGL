#include <glad/glad.h>
#include "CubeMapTexture.hpp"

#include <stb_image/stb_image.h>
#include <Core/Logger.hpp>

namespace Agina {

	CubemapTexture::CubemapTexture(const std::vector<std::string>& facePaths)
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		stbi_set_flip_vertically_on_load(false);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < facePaths.size(); i++)
		{
			unsigned char* data = stbi_load(facePaths[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				GLenum format = GL_RGB;
				if (nrChannels == 1)      format = GL_RED;
				else if (nrChannels == 3) format = GL_RGB;
				else if (nrChannels == 4) format = GL_RGBA;

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				AG_CORE_ERROR("Failed To Load Cubemap Face Texture: {0}", facePaths[i]);
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void CubemapTexture::Bind(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}

	uint32_t CubemapTexture::GetID() const
	{ 
		return m_ID; 
	}

	CubemapTexture::~CubemapTexture()
	{
		glDeleteTextures(1, &m_ID);
	}

}

