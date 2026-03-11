#include <Application/Renderer/Model.hpp>
#include <filesystem>
#include <Application/Renderer/ShaderLibrary.hpp>

Model::Model(const std::string& path)
{
    if(!std::filesystem::exists(path)) std::cerr << "Didn't Find " << path << std::endl; 
    LoadModel(path);
}

void Model::Draw(const SceneData& sceneData, const glm::mat4& model)
{
    for (const auto& mesh : meshes) mesh->Draw(sceneData, model);
}

const std::string& Model::GetName()
{
    return directory;
}

void Model::LoadModel(const std::string& path)
{
    directory = path.substr(0, path.find_last_of('/'));
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ASSIMP ERROR: "
                  << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.m_position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        vertex.m_normal = {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        };

        if (mesh->mTextureCoords[0])
        {
            vertex.m_texCoords = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }
        else
        {
            vertex.m_texCoords = {0.0f, 0.0f};
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::shared_ptr<Texture>> textures;

    unsigned int count = material->GetTextureCount(aiTextureType_DIFFUSE);
    for (unsigned int i = 0; i < count; i++)
    {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, i, &str);
        std::string texturePath = directory + "/" + str.C_Str();
        textures.push_back(Texture::Load(texturePath));
    }

    auto shader = ShaderLibrary::Get("basic");
    auto m_Material = std::make_shared<Material>(shader, textures);
    return std::make_shared<Mesh>(vertices, indices, m_Material);
}

