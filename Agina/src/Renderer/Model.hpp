#pragma once

#include <Renderer/Mesh.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Agina
{
    class Model
    {
    public:
        Model(const std::string& path);

        const std::string& GetName();

    private:
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

        std::string m_Name;
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
    };
}