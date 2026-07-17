#pragma once
#include <memory>
#include <Renderer/Transform.hpp> 
#include <Renderer/Skybox.hpp> 
#include <Renderer/Camera.hpp>
#include <string>
#include <Core/MathTypes.hpp>
#include <Core/UUID.hpp>

namespace Agina {

    class Mesh;
    class Material;
    class Model;

    struct IDComponent
    {
        UUID ID;
    };

    struct BoxCollider 
    {
        Vec3 localMin = Vec3(0.0f);
        Vec3 localMax = Vec3(0.0f);

        Vec3 worldMin = Vec3(0.0f);
        Vec3 worldMax = Vec3(0.0f);

        BoxCollider(const Vec3& min, const Vec3& max) 
            : localMin(min), localMax(max), worldMin(min), worldMax(max) {}
    };

    struct Rigidbody 
    {
        bool isStatic = false;
        Vec3 velocity = Vec3(0.0f);
        Vec3 acceleration = Vec3(0.0f);
        Vec3 forceAccumulator = Vec3(0.0f);
        float mass = 1.0f;
        float gravityScale = 1.0f; 

        void AddForce(const Vec3& force) 
        {
            forceAccumulator += force;
        }

    };

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
