#include <glad/glad.h>
#include "Mesh.hpp"
#include <Core/Logger.hpp>
#include <cmath>

namespace Agina {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		: Mesh(MeshData{ vertices, indices }) 
	{
	}

	Mesh::Mesh(const MeshData& data)
		: m_IndexCount(data.indices.size())
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glBindVertexArray(0);
	}

	Mesh::Mesh(const MeshData& data, MeshType type)
		:m_Type(type), m_IndexCount(data.indices.size())
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	void Mesh::Bind() const { glBindVertexArray(m_VAO); }
	void Mesh::Unbind() const { glBindVertexArray(0); }

	void Mesh::Draw() const
	{
		Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_IndexCount), GL_UNSIGNED_INT, nullptr);
	}

	std::shared_ptr<Mesh> Mesh::Create(MeshType type)
	{
		switch (type)
		{
		case MeshType::TRIANGLE: return std::make_shared<Mesh>(GeometryGenerator::CreateTriangle(), type);
		case MeshType::SPHERE:   return std::make_shared<Mesh>(GeometryGenerator::CreateSphere(1.0f, 32, 16), type);
		case MeshType::TERRAIN:  return std::make_shared<Mesh>(GeometryGenerator::CreateTerrain(50.0f, 50.0f, 64, 64), type);
		case MeshType::QUAD:  return std::make_shared<Mesh>(GeometryGenerator::CreateQuad(), type);
		case MeshType::GRID: return std::make_shared<Mesh>(GeometryGenerator::CreateGrid(), type);
		case MeshType::CUBE: return std::make_shared<Mesh>(GeometryGenerator::CreateCube(), type);
		}
		AG_CORE_WARN("Unknown Built-in Mesh Type Requested");
		return nullptr;
	}
}