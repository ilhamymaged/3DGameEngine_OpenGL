#pragma once

#include <vector>
#include <cstdint>
#include <Core/MathTypes.hpp>

namespace Agina {

	struct Vertex 
	{
		Vec3 pos;
		Vec3 normal;
		Vec2 texCoords;
		Vec3 tangent;
		Vec3 bitangent;
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
		static MeshData CreateQuad();
		static MeshData CreateGrid();
		static MeshData CreateCube();
	};
}
