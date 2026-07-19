#include <glad/glad.h>
#include "Skybox.hpp"
#include <Renderer/CubemapTexture.hpp>
#include <Core/FileSystem.hpp>
#include <vector>
#include <string>

namespace Agina {

	Skybox::Skybox(const std::Ref<CubemapTexture>& cubemap)
		: m_Texture(cubemap)
	{
		float skyboxVertices[] = 
		{
			-1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f
		};

		glCreateVertexArrays(1, &m_VAO);
		glCreateBuffers(1, &m_VBO);

		glNamedBufferStorage(m_VBO, sizeof(skyboxVertices), skyboxVertices, 0);

		glEnableVertexArrayAttrib(m_VAO, 0);
		glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	
		glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, 3 * sizeof(float));
		glVertexArrayAttribBinding(m_VAO, 0, 0);
	}

	void Skybox::Draw() const
	{
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	Skybox::~Skybox()
	{
		if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
		if (m_VBO) glDeleteBuffers(1, &m_VBO);
	}

	void Skybox::Bind(uint32_t slot) const
	{
		if (m_Texture) m_Texture->Bind(slot);
	}

}

