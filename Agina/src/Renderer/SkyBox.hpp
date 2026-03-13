#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace Agina
{
	class SkyBox
	{
	public:
		SkyBox(const std::vector<std::string>& faces);
		~SkyBox();

		void Draw(const glm::mat4& view, const glm::mat4& projection);
	private:

		static unsigned int LoadCubemap(const std::vector<std::string>& faces);
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_CubemapTexture;
	};
}