#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace Agina
{
	class CubemapTexture
	{
	public:
		CubemapTexture(const std::vector<std::string>& facePaths);
		~CubemapTexture();

		void Bind(uint32_t slot = 0) const;
		uint32_t GetID() const;

	private:
		uint32_t m_ID = 0;
	};
}
