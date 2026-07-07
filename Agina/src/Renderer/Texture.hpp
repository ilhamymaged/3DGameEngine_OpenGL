#pragma once

#include <glad/glad.h>
#include <iostream>
#include <unordered_map>
#include <memory>

namespace Agina
{
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();
		void Bind(uint32_t slot = 0) const;
	private:
		uint32_t m_ID;
	};
}
