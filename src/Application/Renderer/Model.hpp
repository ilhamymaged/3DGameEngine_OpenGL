#pragma once

#include <Application/Renderer/Mesh.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Application/Renderer/SceneRenderer.hpp> 

class Model
{
public:
    Model(const std::string& path);
    void Draw(const SceneData& sceneData,const glm::mat4& model);

    const std::string& GetName();

private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string directory;
};