#pragma once
#include <Renderer/Transform.hpp> 
#include <Renderer/Skybox.hpp> 
#include <Renderer/Camera.hpp>
#include <string>
#include <Core/MathTypes.hpp>
#include <Core/UUID.hpp>
#include <Agina.h>
#include <Renderer\Mesh.hpp>
#include <Renderer\Model.hpp>
#include <Renderer\Material.hpp>
#include <Core/AssetManager.hpp>

namespace Agina {

    struct IDComponent  
    {
        UUID ID;
    };

    struct BoxCollider 
    {
        Vec3 localMin = Vec3(-0.5f, -0.5f, -0.5f);
        Vec3 localMax = Vec3(0.5f, 0.5f, 0.5f);

        Vec3 worldMin = Vec3(0.0f);
        Vec3 worldMax = Vec3(0.0f);

        BoxCollider() = default;
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
        std::Ref<Mesh> MeshAsset = AssetManager::LoadMesh(MeshType::CUBE);
        std::Ref<Material> MaterialAsset = Material::Create(MaterialType::LIT);
    };

    struct ModelComponent
    {
        std::Ref<Model> ModelAsset = nullptr;
    };

    struct CameraComponent 
    {
        Camera Cam;
        bool IsPrimary = true;
    };

    struct SkyboxComponent 
    {
        std::Ref<Skybox> skyBoxAsset = nullptr;
        std::Ref<Material> skyBoxMaterialAsset = nullptr;
    };

    struct DirectionalLightComponent 
    {
        Vec3 Target;
    };

    template<typename... Components>
    struct ComponentGroup
    {
        template<typename Func>
        static void ForEach(Func&& func)
        {
            (func.template operator()<Components>(), ...);
        }
    };

    template<typename T>
    static const char* ComponentName();

    using AllComponents = ComponentGroup<Transform, BoxCollider, DirectionalLightComponent,
        CameraComponent, ModelComponent, MeshComponent, Rigidbody, SkyboxComponent>;


    template<>
    const char* ComponentName<Transform>()
    {
        return "Transform";
    }

    template<>
    const char* ComponentName<BoxCollider>()
    {
        return "Box Collider";
    }

    template<>
    const char* ComponentName<DirectionalLightComponent>()
    {
        return "Directional Light";
    }

    template<>
    const char* ComponentName<CameraComponent>()
    {
        return "Camera";
    }

    template<>
    const char* ComponentName<ModelComponent>()
    {
        return "Model";
    }

    template<>
    const char* ComponentName<MeshComponent>()
    {
        return "Mesh";
    }

    template<>
    const char* ComponentName<Rigidbody>()
    {
        return "Rigidbody";
    }

    template<>
    const char* ComponentName<SkyboxComponent>()
    {
        return "Skybox";
    }
}
