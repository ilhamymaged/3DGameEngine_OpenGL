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

    struct MeshComponent 
    {
        std::shared_ptr<Mesh> MeshAsset;
        std::shared_ptr<Material> MaterialAsset;
        std::shared_ptr<Texture> DiffuseTextureAsset = nullptr;
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
