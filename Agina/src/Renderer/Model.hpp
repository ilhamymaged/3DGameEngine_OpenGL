#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Renderer/Mesh.hpp>
#include <Core/MathTypes.hpp>
#include <Agina.h>
#include "Material.hpp"

namespace Agina {

	struct SubMesh
	{
		std::Ref<Mesh> Mesh;
		uint32_t MaterialIndex = 0;
		// Accumulated node transform from the Assimp scene graph (e.g. where
		// this window/brick/leaf sits relative to the model's root). Combined
		// with the entity's own Transform at submit time.
		Mat4 LocalTransform;
	};

	class Model
	{
	public:
		Model(const std::string& path);
		~Model() = default;

		void Draw() const;
		inline const std::vector<SubMesh>& GetSubMeshes() const { return m_SubMeshes; }
		inline const std::vector<std::Ref<Material>>& GetMaterials() const { return m_Materials; }
	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene, const Mat4& parentTransform);
		MeshData ExtractMeshData(aiMesh* mesh);
		// Returns the same Mesh for repeat requests of the same aiMesh within
		// one load (fast path), or for a different aiMesh whose extracted
		// vertex/index data is byte-identical to one already seen (slow path,
		// catches exporters that bake separate vertex data per duplicate
		// instead of linking nodes to a shared mesh block). Either way, e.g.
		// 100 visually-identical windows end up as ONE GPU mesh with 100
		// different LocalTransforms instead of 100 VAOs that could never
		// batch into one instanced draw call.
		std::shared_ptr<Mesh> GetOrCreateMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterials(const aiScene* scene);
		std::Ref<Material> ProcessMaterial(aiMaterial* material);
	private:
		std::vector<SubMesh> m_SubMeshes;
		std::vector<std::Ref<Material>> m_Materials;
		std::unordered_map<const aiMesh*, std::shared_ptr<Mesh>> m_MeshCache;
		std::unordered_map<uint64_t, std::shared_ptr<Mesh>> m_MeshByContentHash;
		std::string m_Directory;
	};
}