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
		: Mesh(data, MeshType::NON_BUILTIN) 
	{
	}

	Mesh::Mesh(const MeshData& data, MeshType type)
		:m_Type(type), m_IndexCount(data.indices.size())
	{
		glCreateVertexArrays(1, &m_VAO);
		glCreateBuffers(1, &m_VBO);
		glCreateBuffers(1, &m_EBO);
	
		glNamedBufferStorage(m_VBO, data.vertices.size() * sizeof(Vertex), data.vertices.data(), 0);
		glNamedBufferStorage(m_EBO, data.indices.size() * sizeof(uint32_t), data.indices.data(), 0);

		glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, sizeof(Vertex));
		glVertexArrayElementBuffer(m_VAO, m_EBO);

		glEnableVertexArrayAttrib(m_VAO, 0);
		glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
		glVertexArrayAttribBinding(m_VAO, 0, 0);

		glEnableVertexArrayAttrib(m_VAO, 1);
		glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
		glVertexArrayAttribBinding(m_VAO, 1, 0);

		glEnableVertexArrayAttrib(m_VAO, 2);
		glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));
		glVertexArrayAttribBinding(m_VAO, 2, 0);

		glEnableVertexArrayAttrib(m_VAO, 3);
		glVertexArrayAttribFormat(m_VAO, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
		glVertexArrayAttribBinding(m_VAO, 3, 0);

		glEnableVertexArrayAttrib(m_VAO, 4);
		glVertexArrayAttribFormat(m_VAO, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, bitangent));
		glVertexArrayAttribBinding(m_VAO, 4, 0);

		//SetupInstanceBuffer();
	}

	//void Mesh::SetupInstanceBuffer()
	//{
	//	glCreateBuffers(1, &m_InstanceVBO);

	//	// binding index 1 = per-instance stream, separate from binding 0 (per-vertex)
	//	glVertexArrayVertexBuffer(m_VAO, 1, m_InstanceVBO, 0, sizeof(Mat4));
	//	glVertexArrayBindingDivisor(m_VAO, 1, 1); // advance once per instance, not per vertex

	//	// A mat4 has to be uploaded as 4 consecutive vec4 attributes (locations 5-8),
	//	// each offset by one row's worth of floats within the instance stride.
	//	for (uint32_t i = 0; i < 4; i++)
	//	{
	//		uint32_t location = 5 + i;
	//		glEnableVertexArrayAttrib(m_VAO, location);
	//		glVertexArrayAttribFormat(m_VAO, location, 4, GL_FLOAT, GL_FALSE, i * 4 * sizeof(float));
	//		glVertexArrayAttribBinding(m_VAO, location, 1);
	//	}
	//}

	//void Mesh::UpdateInstanceBuffer(const std::vector<Mat4>& transforms) const
	//{
	//	if (transforms.empty()) return;

	//	// Sized per-frame: grow (never shrink) so we're not reallocating every
	//	// frame once a batch's steady-state instance count is reached.
	//	if (transforms.size() > m_InstanceCapacity)
	//	{
	//		m_InstanceCapacity = transforms.size();
	//		glNamedBufferData(m_InstanceVBO, m_InstanceCapacity * sizeof(Mat4), nullptr, GL_DYNAMIC_DRAW);
	//	}

	//	glNamedBufferSubData(m_InstanceVBO, 0, transforms.size() * sizeof(Mat4), transforms.data());
	//}

	Mesh::~Mesh()
	{
		if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
		if (m_VBO) glDeleteBuffers(1, &m_VBO);
		if (m_EBO) glDeleteBuffers(1, &m_EBO);
		//if (m_InstanceVBO) glDeleteBuffers(1, &m_InstanceVBO);
	}

	//void Mesh::DrawInstanced(const std::vector<Mat4>& transforms) const
	//{
	//	if (transforms.empty()) return;

	//	UpdateInstanceBuffer(transforms);
	//	Bind();
	//	glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_IndexCount), GL_UNSIGNED_INT,
	//		nullptr, static_cast<GLsizei>(transforms.size()));
	//}

	void Mesh::Bind() const { glBindVertexArray(m_VAO); }
	void Mesh::Unbind() const { glBindVertexArray(0); }

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

	std::string Mesh::MeshTypeToString(MeshType type)
	{
		switch (type)
		{
		case MeshType::TRIANGLE: return "Triangle"; 
		case MeshType::SPHERE:   return "Sphere";
		case MeshType::TERRAIN:  return "Terrain";
		case MeshType::QUAD:  return "Quad";
		case MeshType::GRID: return "Grid";
		case MeshType::CUBE: return "Cube";
		}
		AG_CORE_WARN("Unknown Built-in Mesh Type Requested");
		return "";
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	}

}