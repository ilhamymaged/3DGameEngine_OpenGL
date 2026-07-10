#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <Renderer/Transform.hpp> 
#include <Renderer/Skybox.hpp> 
#include <Renderer/Camera.hpp>
#include <string>

namespace Agina {

    class Mesh;
    class Material;
    class Model;

    struct TagComponent
    {
        std::string tag;
    };

    struct MeshComponent 
    {
        std::shared_ptr<Mesh> MeshAsset;
        std::shared_ptr<Material> MaterialAsset;
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
        std::shared_ptr<Skybox> skyBoxAsset;
        std::shared_ptr<Material> skyBoxMaterialAsset;
    };

    struct DirectionalLightComponent 
    {
        glm::vec3 Position;
        glm::vec3 Target;
    };
}
