#include "Model.hpp"
#include <Renderer/Texture.hpp>
#include <Core/Logger/Logger.hpp>
#include <filesystem>
#include <algorithm>

namespace Agina {

	Model::Model(const std::string& path)
	{
		LoadModel(path);
	}

	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
			aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			AG_CORE_ERROR("Assimp Model Parsing Error: {0}", importer.GetErrorString());
			return;
		}

		m_Directory = std::filesystem::path(path).parent_path().string();
		ProcessNode(scene->mRootNode, scene);

		bool first = true;

		for (auto& mesh : m_Meshes)
		{
			const auto& box = mesh->GetAABB();

			if (first)
			{
				m_AABB = box;
				first = false;
			}
			else
			{
				m_AABB.Min = std::min(m_AABB.Min, box.Min);
				m_AABB.Max = std::max(m_AABB.Max, box.Max);
			}
		}

	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		if (!node || !scene) return;

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh) m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;

			if (mesh->HasNormals() && mesh->mNormals)
			{
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;
			}
			else
			{
				vertex.normal = Vec3(0.0f, 1.0f, 0.0f);
			}

			if (mesh->mTextureCoords[0])
			{
				vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.texCoords = Vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			if (face.mIndices != nullptr && face.mNumIndices > 0)
			{
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}
		}

		std::shared_ptr<Texture2D> meshTexture = nullptr;

		if (scene->mMaterials && mesh->mMaterialIndex < scene->mNumMaterials)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			if (material)
			{
				meshTexture = LoadMaterialTexture(material, aiTextureType_DIFFUSE, m_Directory);
			}
		}

		m_MeshTextures.push_back(meshTexture);

		return std::make_shared<Mesh>(vertices, indices);
	}

	std::shared_ptr<Texture2D> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const std::string& directory)
	{
		if (!mat || mat->GetTextureCount(type) == 0) return nullptr;

		aiString str;
		mat->GetTexture(type, 0, &str);
		std::string fullPath = (std::filesystem::path(directory) / str.C_Str()).string();

		auto it = std::find_if(m_UniqueTextures.begin(), m_UniqueTextures.end(),
			[&fullPath](const std::shared_ptr<Texture2D>& texture) {
				return static_cast<bool>(texture) && texture->GetPath() == fullPath;
			});

		if (it != m_UniqueTextures.end())
		{
			return *it;
		}

		auto newTexture = std::make_shared<Texture2D>(fullPath);
		m_UniqueTextures.push_back(newTexture);
		return newTexture;
	}
}