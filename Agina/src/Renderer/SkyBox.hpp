#pragma once
#include <memory>
#include <cstdint>

namespace Agina {

	class CubemapTexture;

	class Skybox 
	{
	public:
		Skybox();
		~Skybox();

		void Bind(uint32_t slot = 0) const;

		void Draw() const;

	private:
		std::shared_ptr<CubemapTexture> m_Texture;
		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
	};
}
