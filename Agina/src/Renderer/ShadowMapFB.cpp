#include "ShadowMapFB.hpp"

namespace Agina {

	ShadowMapFB::ShadowMapFB(int resolution) : m_Resolution(resolution) 
	{
		glGenFramebuffers(1, &m_FBO);
		glGenTextures(1, &m_DepthTex);
		glBindTexture(GL_TEXTURE_2D, m_DepthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ShadowMapFB::~ShadowMapFB() 
	{
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_DepthTex);
	}

	void ShadowMapFB::BindFramebuffer() 
	{ 
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); 
	}

	void ShadowMapFB::UnbindFramebuffer() 
	{ 
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	}

	void ShadowMapFB::BindTexture(uint32_t slot) 
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_DepthTex);
	}

	int ShadowMapFB::GetResolution() const 
	{ 
		return m_Resolution; 
	}

}
