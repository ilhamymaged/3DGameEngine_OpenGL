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

		CubemapTexture(const CubemapTexture&) = delete;
		CubemapTexture& operator=(const CubemapTexture&) = delete;
		
		CubemapTexture(CubemapTexture&& other) noexcept;
		CubemapTexture& operator=(CubemapTexture&& other) noexcept;

		void Bind(uint32_t slot = 0) const override;
		uint32_t GetID() const override { return m_RendererID; }
		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}
