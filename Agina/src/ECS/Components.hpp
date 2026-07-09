#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <Renderer/Transform.hpp> 
#include <Renderer/Skybox.hpp> 
#include <Renderer/Camera.hpp>

namespace Agina {

    class Mesh;
    class Material;
    class Texture;
    class Model;

    struct TagComponent
    {
        std::string tag;
    };

    struct MeshComponent 
    {
        std::shared_ptr<Mesh> MeshAsset;
        std::shared_ptr<Material> MaterialAsset;
        std::shared_ptr<Texture> DiffuseTextureAsset = nullptr;
    };

    struct ModelComponent 
    {
        std::shared_ptr<Model> ModelAsset;
        std::shared_ptr<Material> MaterialAsset;
    };


    struct CameraComponent 
    {
        Camera Cam;
        bool IsPrimary = true;
    };

    struct SkyboxComponent 
    {
        bool Enabled = true;
    };

    struct DirectionalLightComponent 
    {
        glm::vec3 Position;
        glm::vec3 Target;
    };
}
