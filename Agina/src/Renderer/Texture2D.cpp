#include "Texture2D.hpp"

#include <stb_image/stb_image.h>
#include <Core/Logger.hpp>

namespace Agina {

	Texture2D::Texture2D(const std::string& path, const TextureSpecification& spec)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!data) 
		{
			AG_CORE_ERROR("Failed to load texture at path: {0}", path);
			return;
		}

		m_Width = static_cast<uint32_t>(width);
		m_Height = static_cast<uint32_t>(height);

		// Calculate levels for mipmapping
		GLsizei levels = spec.GenerateMipmaps
			? static_cast<GLsizei>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1
			: 1;

		// OpenGL 4.5 DSA Creation & Allocation
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, levels, spec.InternalFormat, m_Width, m_Height);

		// Parameters
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, spec.MinFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, spec.MagFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, spec.WrapS);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, spec.WrapT);

		// Upload
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, spec.DataFormat, GL_UNSIGNED_BYTE, data);

		if (spec.GenerateMipmaps) 
		{
			glGenerateTextureMipmap(m_RendererID);
		}

		stbi_image_free(data);
	}


	Texture2D::Texture2D(Texture2D&& other) noexcept
		: m_RendererID(other.m_RendererID), m_Width(other.m_Width), m_Height(other.m_Height)
	{
		other.m_RendererID = 0;
		other.m_Width = 0;
		other.m_Height = 0;
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
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

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	Texture2D::~Texture2D()
	{
		if (m_RendererID) glDeleteTextures(1, &m_RendererID);
	}

}
