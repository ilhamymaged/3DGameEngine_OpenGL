#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Renderer/Mesh.hpp>

namespace Agina {

	class Model
	{
	public:
		Model(const std::string& path);
		~Model() = default;

		void Draw() const;

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	private:
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::string m_Directory;
	};
}