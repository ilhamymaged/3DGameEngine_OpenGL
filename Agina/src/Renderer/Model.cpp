#include "Model.hpp"
#include <Core/Logger.hpp>
#include <filesystem>
#include <algorithm>
#include <Core/AssetManager.hpp>

namespace Agina {

	// Assimp matrices are row-major; our Mat4::data is a flat column-major
	// array matching GLM's memory layout (see Math.cpp's ToAgina/ToGLM), so
	// this transposes on the way in.
	static Mat4 ToMat4(const aiMatrix4x4& m)
	{
		Mat4 result;
		result.data[0] = m.a1; result.data[4] = m.a2; result.data[8] = m.a3; result.data[12] = m.a4;
		result.data[1] = m.b1; result.data[5] = m.b2; result.data[9] = m.b3; result.data[13] = m.b4;
		result.data[2] = m.c1; result.data[6] = m.c2; result.data[10] = m.c3; result.data[14] = m.c4;
		result.data[3] = m.d1; result.data[7] = m.d2; result.data[11] = m.d3; result.data[15] = m.d4;
		return result;
	}

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
		LoadMaterials(scene);
		ProcessNode(scene->mRootNode, scene, Mat4());
	}

	void Model::LoadMaterials(const aiScene* scene)
	{
		m_Materials.reserve(scene->mNumMaterials);

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			auto material =
				ProcessMaterial(
					scene->mMaterials[i]
				);

			m_Materials.push_back(material);
		}
	}

	std::Ref<Material> Model::ProcessMaterial(aiMaterial* material)
	{
		auto mat = Material::Create(MaterialType::LIT);

		aiColor3D color;

		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			mat->Set("u_Color", Vec3(color.r, color.g, color.b));
			mat->Set("u_HasColor", true);
		}
		aiString texturePath;

		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS ||
			material->GetTexture(aiTextureType_BASE_COLOR, 0, &texturePath) == AI_SUCCESS)
		{
			std::filesystem::path fullPath = std::filesystem::path(m_Directory) / texturePath.C_Str();
			auto texture = AssetManager::Load2DTexture(fullPath.string(), fullPath.string());

			mat->Set("u_AlbedoTexture", texture);
			mat->Set("u_HasAlbedoTexture", true);
		}
		else
		{
			mat->Set("u_HasAlbedoTexture", false);
		}

		if (material->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
		{
			std::filesystem::path fullPath = std::filesystem::path(m_Directory) / texturePath.C_Str();
			auto texture = AssetManager::Load2DTexture(fullPath.string(), fullPath.string());

			mat->Set("u_NormalTexture", texture);
			mat->Set("u_HasNormalTexture", true);
		}
		else
		{
			mat->Set("u_HasNormalTexture", false);
		}

		if (material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS)
		{
			std::filesystem::path fullPath = std::filesystem::path(m_Directory) / texturePath.C_Str();
			auto texture = AssetManager::Load2DTexture(fullPath.string(), fullPath.string());

			mat->Set("u_SpecularTexture", texture);
			mat->Set("u_HasSpecularTexture", true);
		}
		else
		{
			mat->Set("u_HasSpecularTexture", false);
		}

		return mat;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, const Mat4& parentTransform)
	{
		if (!node || !scene) return;

		// Accumulate this node's local transform on top of everything above
		// it, exactly like Assimp intends: two nodes referencing the SAME
		// aiMesh with different mTransformation are two placements of one
		// piece of geometry, not two different meshes.
		Mat4 worldTransform = parentTransform * ToMat4(node->mTransformation);

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh)
			{
				SubMesh subMesh;
				subMesh.Mesh = GetOrCreateMesh(mesh, scene);
				subMesh.MaterialIndex = mesh->mMaterialIndex;
				subMesh.LocalTransform = worldTransform;

				m_SubMeshes.push_back(subMesh);
			}
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene, worldTransform);
	}

	// FNV-1a 64-bit over the raw vertex+index bytes. Vertex is an all-float
	// POD (Vec3/Vec2 members only), so no padding to worry about - identical
	// geometry produces identical bytes regardless of which aiMesh it came
	// from. This is what lets us dedup exporters that bake fully separate
	// (but byte-identical) vertex data per copy, not just ones that link
	// multiple nodes to the same aiMesh block.
	static uint64_t HashMeshData(const MeshData& data)
	{
		uint64_t hash = 14695981039346656037ull;
		auto mix = [&hash](const void* bytes, size_t size)
			{
				const uint8_t* p = static_cast<const uint8_t*>(bytes);
				for (size_t i = 0; i < size; i++)
				{
					hash ^= p[i];
					hash *= 1099511628211ull;
				}
			};

		mix(data.vertices.data(), data.vertices.size() * sizeof(Vertex));
		mix(data.indices.data(), data.indices.size() * sizeof(uint32_t));
		return hash;
	}

	std::shared_ptr<Mesh> Model::GetOrCreateMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Fast path: this exact aiMesh block was already processed (covers
		// exporters that link multiple nodes to one shared mesh).
		auto ptrIt = m_MeshCache.find(mesh);
		if (ptrIt != m_MeshCache.end())
			return ptrIt->second;

		MeshData data = ExtractMeshData(mesh);
		uint64_t hash = HashMeshData(data);

		// Slow-but-correct path: different aiMesh block, but byte-identical
		// geometry to one we've already uploaded (covers exporters that bake
		// separate vertex data per duplicate).
		auto contentIt = m_MeshByContentHash.find(hash);
		if (contentIt != m_MeshByContentHash.end())
		{
			m_MeshCache[mesh] = contentIt->second;
			return contentIt->second;
		}

		auto created = std::make_shared<Mesh>(data);
		m_MeshCache[mesh] = created;
		m_MeshByContentHash[hash] = created;
		return created;
	}

	MeshData Model::ExtractMeshData(aiMesh* mesh)
	{
		MeshData data;
		auto& vertices = data.vertices;
		auto& indices = data.indices;

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

			if (mesh->HasTangentsAndBitangents() && mesh->mTangents && mesh->mBitangents)
			{
				vertex.tangent.x = mesh->mTangents[i].x;
				vertex.tangent.y = mesh->mTangents[i].y;
				vertex.tangent.z = mesh->mTangents[i].z;

				vertex.bitangent.x = mesh->mBitangents[i].x;
				vertex.bitangent.y = mesh->mBitangents[i].y;
				vertex.bitangent.z = mesh->mBitangents[i].z;
			}
			else
			{
				vertex.tangent = Vec3(1.0f, 0.0f, 0.0f);
				vertex.bitangent = Vec3(0.0f, 1.0f, 0.0f);
			}

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
		return data;
	}

	void Model::Draw() const
	{
		for (auto& subMesh : m_SubMeshes)
		{
			//subMesh.Mesh->DrawInstanced({ subMesh.LocalTransform });
			subMesh.Mesh->Draw();
		}
	}
}