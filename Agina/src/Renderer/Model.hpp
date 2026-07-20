#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Renderer/Mesh.hpp>
#include <Agina.h>
#include "Material.hpp"

namespace Agina {

	struct SubMesh
	{
		std::Ref<Mesh> Mesh;
		uint32_t MaterialIndex = 0;
	};

	class Model
	{
	public:
		Model(const std::string& path);
		~Model() = default;

		void Draw() const;
		inline const std::vector<SubMesh>& GetSubMeshes() const { return m_SubMeshes; }
		inline const std::vector<std::Ref<Material>>& GetMaterials() const {return m_Materials;}
	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterials(const aiScene* scene);
		std::Ref<Material> ProcessMaterial(aiMaterial* material);
	private:
		std::vector<SubMesh> m_SubMeshes;
		std::vector<std::Ref<Material>> m_Materials;
		std::string m_Directory;
	};
}