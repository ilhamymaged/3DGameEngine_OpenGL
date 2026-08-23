#include <Renderer/Framebuffer.hpp>
#include <glad/glad.h> 

namespace Agina {

	namespace Utils {
		static GLenum TextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_R32I;
			default: return 0;
			}
		}
	}

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto format : m_Specification.Attachments.Attachments)
		{
			if (format.TextureFormat == FramebufferTextureFormat::DEPTH24STENCIL8 ||
				format.TextureFormat == FramebufferTextureFormat::DEPTH_COMPONENT)
			{
				m_DepthAttachmentSpecification = format;
			}
			else
			{
				m_ColorAttachmentSpecifications.push_back(format);
			}
		}

		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void Framebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		if (!m_ColorAttachmentSpecifications.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			glGenTextures(m_ColorAttachments.size(), m_ColorAttachments.data());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);

				if (m_ColorAttachmentSpecifications[i].TextureFormat == FramebufferTextureFormat::RGBA8)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				}
				else if (m_ColorAttachmentSpecifications[i].TextureFormat == FramebufferTextureFormat::RED_INTEGER)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Specification.Width, m_Specification.Height, 0, GL_RED_INTEGER, GL_INT, nullptr);
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorAttachments[i], 0);
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			glGenTextures(1, &m_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);

			if (m_DepthAttachmentSpecification.TextureFormat == FramebufferTextureFormat::DEPTH24STENCIL8)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
			}
			else if (m_DepthAttachmentSpecification.TextureFormat == FramebufferTextureFormat::DEPTH_COMPONENT)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
			}
		}

		// 3. Setup draw buffers array based on color attachment count
		if (m_ColorAttachments.size() > 1)
		{
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0) return;
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearBufferiv(GL_COLOR, attachmentIndex, &value);
	}
}
