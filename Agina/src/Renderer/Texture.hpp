#pragma once

#include <glad/glad.h>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "ITexture.hpp"

namespace Agina
{
	class Texture2D : public ITexture
	{
	public:
		Texture2D(const std::string& path);
		~Texture2D();
		void Bind(uint32_t slot = 0) override;

		inline const std::string& GetPath() { return m_Path; }
		inline const uint32_t GetID() { return m_ID; }

	private:
		uint32_t m_ID;
		std::string m_Path;
	};
}
