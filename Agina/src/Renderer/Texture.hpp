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
		void Bind() const;
	private:
		uint32_t m_ID;
	};
}
