#include "Mesh.hpp"
#include <glad/glad.h>
#include <Core/Logger/Logger.hpp>
#include <glm/ext/scalar_constants.hpp> 

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
        else if (type == MeshType::SPHERE)   return Mesh::Sphere();
		else if (type == MeshType::TERRAIN)   return Mesh::Terrain();
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

    std::shared_ptr<Mesh> Mesh::Sphere(float radius, uint32_t sectors, uint32_t stacks)
    {
        // Caching the mesh so it only generates once
        static std::shared_ptr<Mesh> sphere = [=]()
            {
                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;

                float x, y, z, xy;                             // vertex position
                float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
                float s, t;                                    // vertex texCoords

                float sectorStep = 2 * glm::pi<float>() / sectors;
                float stackStep = glm::pi<float>() / stacks;
                float sectorAngle, stackAngle;

                // 1. Generate Vertices, Normals, and UVs
                for (uint32_t i = 0; i <= stacks; ++i)
                {
                    stackAngle = glm::pi<float>() / 2 - i * stackStep; // starting from pi/2 to -pi/2
                    xy = radius * cosf(stackAngle);             // r * cos(u)
                    z = radius * sinf(stackAngle);              // r * sin(u)

                    for (uint32_t j = 0; j <= sectors; ++j)
                    {
                        sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                        // Position
                        x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                        y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

                        // Normal
                        nx = x * lengthInv;
                        ny = y * lengthInv;
                        nz = z * lengthInv;

                        // Texture Coordinates
                        s = (float)j / sectors;
                        t = (float)i / stacks;

                        vertices.push_back({ {x, y, z}, {nx, ny, nz}, {s, t} });
                    }
                }

                // 2. Generate Indices (Triangles)
                uint32_t k1, k2;
                for (uint32_t i = 0; i < stacks; ++i)
                {
                    k1 = i * (sectors + 1);     // beginning of current stack
                    k2 = k1 + sectors + 1;      // beginning of next stack

                    for (uint32_t j = 0; j < sectors; ++j, ++k1, ++k2)
                    {
                        // 2 triangles per sector (except for top and bottom stacks, but 
                        // generating degenerate triangles here keeps code simple and clean)

                        // Triangle 1: k1 -> k2 -> k1+1
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);

                        // Triangle 2: k1+1 -> k2 -> k2+1
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }

                return std::make_shared<Mesh>(vertices, indices);
            }();

        return sphere;
    }

	std::shared_ptr<Mesh> Mesh::Terrain(float width, float depth, uint32_t m, uint32_t n)
	{
		static std::shared_ptr<Mesh> terrain = [&]()
			{
				std::vector<Vertex> vertices;
				std::vector<uint32_t> indices;

				uint32_t vertexCount = m * n;
				uint32_t faceCount = (m - 1) * (n - 1) * 2;

				float halfWidth = 0.5f * width;
				float halfDepth = 0.5f * depth;

				float dx = width / (n - 1);
				float dz = depth / (m - 1);

				float du = 1.0f / (n - 1);
				float dv = 1.0f / (m - 1);

				vertices.resize(vertexCount);
				for (uint32_t i = 0; i < m; ++i)
				{
					float z = halfDepth - i * dz;
					for (uint32_t j = 0; j < n; ++j)
					{
						float x = -halfWidth + j * dx;

						// Procedural height formula. You can change this to 0.0f 
						// if you want a flat grid to deform using a vertex shader!
						//float y = 2.5f * sinf(x * 0.2f) * cosf(z * 0.2f);
						float y = 0.0f;

						uint32_t index = i * n + j;
						vertices[index].pos = glm::vec3(x, y, z);
						vertices[index].normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default up-vector
						vertices[index].texCoords = glm::vec2(j * du, i * dv);
					}
				}

				indices.resize(faceCount * 3);
				uint32_t k = 0;
				for (uint32_t i = 0; i < m - 1; ++i)
				{
					for (uint32_t j = 0; j < n - 1; ++j)
					{
						indices[k] = i * n + j;
						indices[k + 1] = i * n + j + 1;
						indices[k + 2] = (i + 1) * n + j;

						indices[k + 3] = (i + 1) * n + j;
						indices[k + 4] = i * n + j + 1;
						indices[k + 5] = (i + 1) * n + j + 1;

						k += 6;
					}
				}

				return std::make_shared<Mesh>(vertices, indices);
			}();

		return terrain;
	}

}

