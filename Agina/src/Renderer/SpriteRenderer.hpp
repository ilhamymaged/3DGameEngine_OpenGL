#pragma once

#include <stdint.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include <Renderer/Texture.hpp>
#include <Renderer/Shader.hpp>

namespace Agina {

	class SpriteRenderer
	{
	public:
		SpriteRenderer();
	
		void DrawSprite(Texture& texture, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f)
			, glm::vec2 pos = glm::vec2(0.0f, 0.0f), glm::vec2 size = glm::vec2(50.0f, 50.0f),
			float rotation = 0.0f);

	private:
		uint32_t m_VAO;
		Shader& m_Shader;
	};

}