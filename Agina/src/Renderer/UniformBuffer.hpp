#pragma once
#include <cstdint>

namespace Agina {

	class UniformBuffer
	{
	public:
		UniformBuffer(uint32_t size, uint32_t binding);
		~UniformBuffer();

		void SetData(const void* data, uint32_t size);

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Binding = 0;
	};
}