#pragma once
#include <memory>
#include <Renderer/Transform.hpp> 
#include <Renderer/Skybox.hpp> 
#include <Renderer/Camera.hpp>
#include <string>
#include <Core/Math/MathTypes.hpp>

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
        Vec3 Position;
        Vec3 Target;
    };
}
