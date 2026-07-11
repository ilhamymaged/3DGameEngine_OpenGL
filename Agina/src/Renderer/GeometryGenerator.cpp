#include "GeometryGenerator.hpp"
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/noise.hpp>
#include <Core/Math/Math.hpp>

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

	MeshData GeometryGenerator::CreateQuad()
	{
		MeshData data;
		data.vertices =
		{
			{{-1.f, -1.0f,  0.0f}, {0.0f,  0.0f,  1.0f}, {0.0f,  0.0f}},
			{{ 1.0f, -1.0f,  0.0f}, {0.0f,  0.0f,  1.0f}, {1.0f,  0.0f}},
			{{-1.0f,  1.0f,  0.0f}, {0.0f,  0.0f,  1.0f}, {0.0f,  1.0f}},
			{{1.0f,  1.0f,  0.0f}, {0.0f,  0.0f,  1.0f}, {1.0f,  1.0f}}
		};

		data.indices = {0, 1, 2, 2, 1, 3};
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

        float dx = width / (n - 1);
        float dz = depth / (m - 1);

        float du = 1.0f / (n - 1);
        float dv = 1.0f / (m - 1);

        data.vertices.resize(vertexCount);
        for (uint32_t i = 0; i < m; ++i)
        {
            float z = halfDepth - i * dz;
            for (uint32_t j = 0; j < n; ++j)
            {
                float x = -halfWidth + j * dx;

                float frequency = 0.015f;

                float n1 = glm::perlin(glm::vec2(x * frequency, z * frequency));
                float n2 = glm::perlin(glm::vec2(x * frequency * 2.5f, z * frequency * 2.5f)) * 0.4f;
                float n3 = glm::perlin(glm::vec2(x * frequency * 6.0f, z * frequency * 6.0f)) * 0.1f;

                float rawElevation = (n1 + n2 + n3 + 1.5f) / 3.0f;
                rawElevation = glm::clamp(rawElevation, 0.0f, 1.0f);

                float finalHeightFactor = glm::pow(rawElevation, 2.2f);

                float maxMountainHeight = 8.0f;
                float y = finalHeightFactor * maxMountainHeight;

                uint32_t index = i * n + j;
                data.vertices[index].pos = Vec3(x, y, z);
				float textureScale = 0.5f; 
				data.vertices[index].texCoords = Vec2(x * textureScale, z * textureScale);
            }
        }

        data.indices.resize(faceCount * 3);
        uint32_t k = 0;
        for (uint32_t i = 0; i < m - 1; ++i)
        {
            for (uint32_t j = 0; j < n - 1; ++j)
            {
                data.indices[k] = i * n + j;
                data.indices[k + 1] = i * n + j + 1;
                data.indices[k + 2] = (i + 1) * n + j;

                data.indices[k + 3] = (i + 1) * n + j;
                data.indices[k + 4] = i * n + j + 1;
                data.indices[k + 5] = (i + 1) * n + j + 1;

                k += 6;
            }
        }

        for (auto& vertex : data.vertices)
        {
            vertex.normal = Vec3(0.0f);
        }

        for (size_t i = 0; i < data.indices.size(); i += 3)
        {
            uint32_t idx0 = data.indices[i];
            uint32_t idx1 = data.indices[i + 1];
            uint32_t idx2 = data.indices[i + 2];

            Vec3 v0 = data.vertices[idx0].pos;
            Vec3 v1 = data.vertices[idx1].pos;
			Vec3 v2 = data.vertices[idx2].pos;

			Vec3 edge1 = v1 - v0;
			Vec3 edge2 = v2 - v0;
			Vec3 faceNormal = Math::Cross(edge1, edge2);

            data.vertices[idx0].normal += faceNormal;
            data.vertices[idx1].normal += faceNormal;
            data.vertices[idx2].normal += faceNormal;
        }

        for (auto& vertex : data.vertices)
        {
            vertex.normal = Math::Normalize(vertex.normal);
        }

        return data;
	}

}