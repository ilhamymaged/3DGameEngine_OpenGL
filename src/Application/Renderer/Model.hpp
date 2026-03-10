#pragma once

#include <Application/Renderer/Mesh.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const std::string& path);
    void Draw(glm::mat4& view, glm::mat4& proj, glm::mat4& model);

private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string directory;
};