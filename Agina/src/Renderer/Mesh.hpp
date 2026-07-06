#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace Agina {

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	enum class MeshType
	{
		TRIANGLE,
	};

	class Mesh 
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		void Draw() const;	
	
		//Built-in Meshes
		static std::shared_ptr<Mesh> Create(MeshType type);

	private:
		uint32_t m_VAO;
		uint32_t m_VBO;
		uint32_t m_EBO;
		size_t m_IndexCount;
		
	private:
		static std::shared_ptr<Mesh> Triangle();
	};

}

