#pragma once

#include <vector>
#include <memory>
#include <Renderer/Transform.hpp>
#include "GeometryGenerator.hpp"
#include <Core/MathTypes.hpp>

namespace Agina {

	enum class MeshType
	{
		TRIANGLE,
		SPHERE,
		TERRAIN,
		QUAD,
		GRID,
		CUBE,
		NON_BUILTIN
	};

	class Mesh
	{
	public:
		Mesh(const MeshData& data);
		Mesh(const MeshData& data, MeshType type);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		~Mesh();

		void Bind() const;
		void Unbind() const;
		void Draw() const;

		MeshType GetMeshType() const { return m_Type; }

		static std::shared_ptr<Mesh> Create(MeshType type);
	private:
		uint32_t m_VAO;
		uint32_t m_VBO;
		uint32_t m_EBO;
		size_t m_IndexCount;
		MeshType m_Type = MeshType::NON_BUILTIN;
	};

}