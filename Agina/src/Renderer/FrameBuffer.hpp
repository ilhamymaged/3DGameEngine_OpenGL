#pragma once
#include <glad/glad.h>
#include <iostream>
#include <algorithm>

namespace Agina
{
    class Framebuffer {
    public:
        Framebuffer(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height)
        {
            Init();
        }

        ~Framebuffer() 
        {
            Delete();
        }

        void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); }
        void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

        void Resize(uint32_t newWidth, uint32_t newHeight)
        {
            newWidth = std::max(1u, newWidth);
            newHeight = std::max(1u, newHeight);

            if (m_Width == newWidth && m_Height == newHeight)
                return;

            m_Width = newWidth;
            m_Height = newHeight;

            Delete();
            Init();
        }

        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

    private:
        void Init()
        {
            glCreateFramebuffers(1, &m_RendererID);
            glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

            glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
            glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

            glCreateRenderbuffers(1, &m_DepthAttachment);
            glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
                GL_RENDERBUFFER, m_DepthAttachment);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cerr << "[Framebuffer] ERROR: Framebuffer is incomplete!" << std::endl;

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void Delete()
        {
            if (m_RendererID) glDeleteFramebuffers(1, &m_RendererID);
            if (m_ColorAttachment) glDeleteTextures(1, &m_ColorAttachment);
            if (m_DepthAttachment) glDeleteRenderbuffers(1, &m_DepthAttachment);

            m_RendererID = 0;
            m_ColorAttachment = 0;
            m_DepthAttachment = 0;
        }

    private:
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0;
        uint32_t m_DepthAttachment = 0;
    };
}