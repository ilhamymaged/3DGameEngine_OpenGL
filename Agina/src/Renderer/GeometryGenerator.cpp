#include "GeometryGenerator.hpp"
#include <glm/ext/scalar_constants.hpp>
#include <cmath>

namespace Agina
{
	MeshData GeometryGenerator::CreateTriangle() 
	{
		MeshData data;
		data.vertices = 
		{
			{{-0.5f, -0.5f, 0.0f}, {0,0,1}, {0,0}},
			{{ 0.5f, -0.5f, 0.0f}, {0,0,1}, {1,0}},
			{{ 0.0f,  0.5f, 0.0f}, {0,0,1}, {0.5f,1}}
		};
		data.indices = { 0, 1, 2 };
		return data;
	}

	MeshData GeometryGenerator::CreateSphere(float radius, uint32_t sectors, uint32_t stacks) 
	{
		MeshData data;
		float x, y, z, xy;
		float nx, ny, nz, lengthInv = 1.0f / radius;
		float s, t;

		float sectorStep = 2 * glm::pi<float>() / sectors;
		float stackStep = glm::pi<float>() / stacks;

		for (uint32_t i = 0; i <= stacks; ++i) 
		{
			float stackAngle = glm::pi<float>() / 2 - i * stackStep;
			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);

			for (uint32_t j = 0; j <= sectors; ++j) 
			{
				float sectorAngle = j * sectorStep;
				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);

				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;

				s = (float)j / sectors;
				t = (float)i / stacks;

				data.vertices.push_back({ {x, y, z}, {nx, ny, nz}, {s, t} });
			}
		}

		for (uint32_t i = 0; i < stacks; ++i) 
		{
			uint32_t k1 = i * (sectors + 1);
			uint32_t k2 = k1 + sectors + 1;

			for (uint32_t j = 0; j < sectors; ++j, ++k1, ++k2) 
			{
				data.indices.push_back(k1);
				data.indices.push_back(k2);
				data.indices.push_back(k1 + 1);

				data.indices.push_back(k1 + 1);
				data.indices.push_back(k2);
				data.indices.push_back(k2 + 1);
			}
		}
		return data;
	}

	MeshData GeometryGenerator::CreateTerrain(float width, float depth, uint32_t m, uint32_t n) 
	{
		MeshData data;
		uint32_t vertexCount = m * n;
		uint32_t faceCount = (m - 1) * (n - 1) * 2;

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		// Distance between vertices
		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		// UV texture coordinate increments
		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		// 1. Generate a perfectly flat grid layout
		data.vertices.resize(vertexCount);
		for (uint32_t i = 0; i < m; ++i) 
		{
			float z = halfDepth - i * dz;
			for (uint32_t j = 0; j < n; ++j) 
			{
				float x = -halfWidth + j * dx;

				// PERFECTLY FLAT: y is exactly 0.0f
				float y = 0.0f;

				uint32_t index = i * n + j;
				data.vertices[index].pos = glm::vec3(x, y, z);

				// Since it's flat, all normals point straight UP
				data.vertices[index].normal = glm::vec3(0.0f, 1.0f, 0.0f);

				data.vertices[index].texCoords = glm::vec2(j * du, i * dv);
			}
		}

		// 2. Generate Triangles connecting the grid quads
		data.indices.resize(faceCount * 3);
		uint32_t k = 0;
		for (uint32_t i = 0; i < m - 1; ++i) 
		{
			for (uint32_t j = 0; j < n - 1; ++j) 
			{
				// Triangle 1
				data.indices[k] = i * n + j;
				data.indices[k + 1] = i * n + j + 1;
				data.indices[k + 2] = (i + 1) * n + j;

				// Triangle 2
				data.indices[k + 3] = (i + 1) * n + j;
				data.indices[k + 4] = i * n + j + 1;
				data.indices[k + 5] = (i + 1) * n + j + 1;

				k += 6; 
			}
		}
		return data;
	}

}