#pragma once
#include <glad/glad.h>
#include <cinttypes>

namespace Agina {

	class ShadowMapFB 
	{
	public:
		ShadowMapFB(int resolution);
		~ShadowMapFB();

		void BindFramebuffer();
		void UnbindFramebuffer();
		void BindTexture(uint32_t slot);
		int GetResolution() const;
	private:
		uint32_t m_FBO = 0;
		uint32_t m_DepthTex = 0;
		int m_Resolution;
	};
}
