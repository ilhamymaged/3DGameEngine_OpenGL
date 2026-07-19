#pragma once

#include <glad/glad.h>
#include <string>
#include <Renderer/ITexture.hpp>

namespace Agina {

	struct TextureSpecification 
	{
		GLenum InternalFormat = GL_RGBA8;
		GLenum DataFormat = GL_RGBA;
		GLenum WrapS = GL_REPEAT;
		GLenum WrapT = GL_REPEAT;
		GLenum MinFilter = GL_LINEAR_MIPMAP_LINEAR;
		GLenum MagFilter = GL_LINEAR;
		bool   GenerateMipmaps = true;
	};

	class Texture2D : public ITexture
	{
	public:
		Texture2D(const std::string& path, const TextureSpecification& spec = TextureSpecification());
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		Texture2D(Texture2D&& other) noexcept;
		Texture2D& operator=(Texture2D&& other) noexcept;

		inline uint32_t GetID() const override { return m_RendererID;}
		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override  { return m_Height; }
		void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width, m_Height;
	};

}
