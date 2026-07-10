#pragma once

#include <vector>
#include <memory>
#include <Renderer/Transform.hpp>
#include "GeometryGenerator.hpp"
#include <glm/glm.hpp>

namespace Agina {

	enum class MeshType
	{
		TRIANGLE,
		SPHERE,
		TERRAIN,
		QUAD
	};

	struct AABB
	{
		glm::vec3 Min;
		glm::vec3 Max;
	};

	class Mesh
	{
	public:
		Mesh(const MeshData& data);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		~Mesh();

		void Bind() const;
		void Unbind() const;
		void Draw() const;

		static std::shared_ptr<Mesh> Create(MeshType type);
		const AABB& GetAABB() const { return m_AABB; }

	private:
		AABB ComputeAABB(const std::vector<Vertex>& vertices);

	private:
		uint32_t m_VAO;
		uint32_t m_VBO;
		uint32_t m_EBO;
		size_t m_IndexCount;
		AABB m_AABB;
	};

}