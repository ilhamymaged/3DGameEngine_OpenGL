#pragma once

#include <cinttypes>

namespace Agina {

	class ITexture
	{
	public:
		virtual void Bind(uint32_t slot) = 0;
	};

}

