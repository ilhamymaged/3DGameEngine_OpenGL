#pragma once
#include <stdint.h>
#include <initializer_list>
#include <vector>

namespace Agina {

	enum class FramebufferTextureFormat
	{
		None = 0,

		RGBA8,
		RED_INTEGER, 

		DEPTH24STENCIL8,
		DEPTH_COMPONENT 
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) 
		{
		}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) 
		{
		}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Bind();
		void Unbind();
		void Resize(uint32_t width, uint32_t height);

		int ReadPixel(uint32_t attachmentIndex, int x, int y);

		// Clear a specific integer attachment to a default value
		void ClearAttachment(uint32_t attachmentIndex, int value);

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const { return m_ColorAttachments[index]; }
		uint32_t GetDepthAttachmentRendererID() const { return m_DepthAttachment; }
		const FramebufferSpecification& GetSpecification() const { return m_Specification; }

	private:
		void Invalidate();

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;

	};
}