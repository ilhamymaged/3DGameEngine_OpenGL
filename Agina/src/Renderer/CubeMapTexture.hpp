#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "ITexture.hpp"

namespace Agina {

	class CubemapTexture : public ITexture
	{
	public:
		CubemapTexture(const std::vector<std::string>& facePaths);
		~CubemapTexture();

		void Bind(uint32_t slot = 0) override;
		uint32_t GetID() const;

	private:
		uint32_t m_ID = 0;
	};
}
