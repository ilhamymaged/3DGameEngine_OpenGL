#pragma once
#include <Agina.h>

namespace Agina {

	class CubemapTexture;

	class Skybox 
	{
	public:
		Skybox(const std::Ref<CubemapTexture>& cubemap);
		~Skybox();

		void Bind(uint32_t slot = 0) const;
		void Draw() const;

	private:
		std::Ref<CubemapTexture> m_Texture;
		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
	};
}
