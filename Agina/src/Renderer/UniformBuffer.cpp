#include "UniformBuffer.hpp"

#include <glad/glad.h>

namespace Agina {

	UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding) : m_Binding(binding)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	UniformBuffer::~UniformBuffer() 
	{ 
		glDeleteBuffers(1, &m_RendererID); 
	}

	void UniformBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	}

}

