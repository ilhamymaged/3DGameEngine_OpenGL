#pragma once

#include <glad/glad.h>
#include <iostream>
#include <unordered_map>

namespace Agina
{
	class Texture
	{
	public:
		static std::shared_ptr<Texture> Load(const std::string& path);
		void Bind() const;
	private:
		Texture(const std::string& path);
		uint32_t m_ID;
		static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	};
}
