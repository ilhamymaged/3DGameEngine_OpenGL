#include "Model.hpp"
#include <Renderer/Texture.hpp>
#include <Core/Logger.hpp>
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
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		if (!node || !scene) return;

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh) m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) ProcessNode(node->mChildren[i], scene);
	}

	std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			if (mesh->HasPositions() && mesh->mVertices)
			{
				vertex.pos.x = mesh->mVertices[i].x;
				vertex.pos.y = mesh->mVertices[i].y;
				vertex.pos.z = mesh->mVertices[i].z;
			}
			else vertex.pos = Vec3(0.0f, 0.0f, 0.0f);

			if (mesh->HasNormals() && mesh->mNormals)
			{
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;
			}
			else vertex.normal = Vec3(0.0f, 1.0f, 0.0f);

			if (mesh->mTextureCoords[0])
			{
				vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else vertex.texCoords = Vec2(0.0f, 0.0f);

			if (mesh->HasTangentsAndBitangents() && mesh->mTangents)
			{
				vertex.tangent.x = mesh->mTangents[i].x;
				vertex.tangent.y = mesh->mTangents[i].y;
				vertex.tangent.z = mesh->mTangents[i].z;
			}
			else vertex.tangent = Vec3(1.0f, 0.0f, 0.0f); 

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			if (face.mIndices != nullptr && face.mNumIndices > 0)
			{
				for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
			}
		}
		return std::make_shared<Mesh>(vertices, indices);
	}

	void Model::Draw() const
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->Draw();
		}
	}
}