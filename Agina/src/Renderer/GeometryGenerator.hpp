#pragma once

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

namespace Agina {

	struct Vertex 
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	struct MeshData 
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class GeometryGenerator 
	{
	public:
		static MeshData CreateTriangle();
		static MeshData CreateSphere(float radius, uint32_t sectors, uint32_t stacks);
		static MeshData CreateTerrain(float width, float depth, uint32_t m, uint32_t n);
	};
}
