#include "Mesh.hpp"
#include <glad/glad.h>
#include <Core/Logger/Logger.hpp>

namespace Agina {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		: m_VAO(0), m_VBO(0), m_EBO(0), m_IndexCount(indices.size())
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Assuming In All Shaders Pos(0), Normal(1), TexCoords(2)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		//Good Practice
		glBindVertexArray(0);
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	}
	std::shared_ptr<Mesh> Mesh::Create(MeshType type)
	{
		if (type == MeshType::TRIANGLE) return Mesh::Triangle();
		else
		{
			AG_CORE_WARN("UnSupported Built-in Mesh Type");
			return nullptr;
		}
	}
	std::shared_ptr<Mesh> Mesh::Triangle()
	{
		static std::shared_ptr<Mesh> triangle = []()
			{
				std::vector<Vertex> vertices =
				{
					{{-0.5f, -0.5f, 0.0f}, {0,0,1}, {0,0}},
					{{ 0.5f, -0.5f, 0.0f}, {0,0,1}, {1,0}},
					{{ 0.0f,  0.5f, 0.0f}, {0,0,1}, {0.5f,1}}
				};

				std::vector<uint32_t> indices = { 0,1,2 };
				return std::make_shared<Mesh>(vertices, indices);
			}();

		return triangle;
	}
}

