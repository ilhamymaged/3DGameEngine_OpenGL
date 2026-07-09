#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <Renderer/Mesh.hpp>

namespace Agina {

	class Mesh;
	class Texture;

	class Model
	{
	public:
		Model(const std::string& path);
		~Model() = default;

		inline const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
		inline const std::vector<std::shared_ptr<Texture>>& GetTextures() const { return m_MeshTextures; }

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::shared_ptr<Texture> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const std::string& directory);
	private:
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::vector<std::shared_ptr<Texture>> m_MeshTextures;
		std::vector<std::shared_ptr<Texture>> m_UniqueTextures;
		std::string m_Directory;
		AABB m_AABB;
	};
}