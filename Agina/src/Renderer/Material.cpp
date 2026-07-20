#include "Material.hpp"

#include <Core/Logger.hpp>
#include <Core/FileSystem.hpp>
#include <Core/AssetManager.hpp>
#include <type_traits> 

namespace Agina {

	Material::Material(std::shared_ptr<Shader> shader, MaterialType type)
		: m_Shader(shader), m_MaterialType(type)  {}

	const MaterialValue& Material::Get(const std::string& name) const
	{
		return m_Parameters.at(name);
	}

	void Material::Set(const std::string& name, const MaterialValue& value)
	{
		m_Parameters[name] = value;
	}

	std::shared_ptr<Material> Material::Create(MaterialType type)
	{
        if (type == MaterialType::LIT) return Material::Lit();
        else if (type == MaterialType::UNLIT) return Material::UnLit();
        else if (type == MaterialType::SKYBOX) return Material::SkyBox();
        else if (type == MaterialType::GRID) return Material::Grid();
		else
		{
			AG_CORE_WARN("UnSupported Built-in Material Type");
			return nullptr;
		}
	}

	std::shared_ptr<Material> Material::Lit()
	{
		return std::make_shared<Material>(AssetManager::LoadShader("lit",
			(FileSystem::EngineAssets() / "shaders/lit").string()),
			MaterialType::LIT);
	}

	std::shared_ptr<Material> Material::UnLit()
	{
		return std::make_shared<Material>(AssetManager::LoadShader("unlit",
			(FileSystem::EngineAssets() / "shaders/unlit").string()),
			MaterialType::UNLIT);
	}

	std::shared_ptr<Material> Material::SkyBox()
	{
		return std::make_shared<Material>(AssetManager::LoadShader("skyBox", (FileSystem::EngineAssets() / "shaders/skybox").string()),
			MaterialType::SKYBOX);
	}

    std::shared_ptr<Material> Material::Grid()
    {
        return std::make_shared<Material>(AssetManager::LoadShader("grid", (FileSystem::EngineAssets() / "shaders/grid").string()),
            MaterialType::GRID);
    }

    void Material::Bind() const
    {
        m_Shader->Use();

		uint32_t textureSlot = 0;

        for (auto& [name, value] : m_Parameters)
        {
            std::visit([&](auto&& v)
                {
                    using T = std::decay_t<decltype(v)>;

					if constexpr (std::is_same_v<T, bool>)
						m_Shader->setInt(name, v);
                    else if constexpr (std::is_same_v<T, int>)
                        m_Shader->setInt(name, v);
                    else if constexpr (std::is_same_v<T, float>)
                        m_Shader->setFloat(name, v);
                    else if constexpr (std::is_same_v<T, Vec3>)
                        m_Shader->setVec3(name, v);
                    else if constexpr (std::is_same_v<T, Vec4>)
                        m_Shader->setVec4(name, v);
                    else if constexpr (std::is_same_v<T, Mat4>)
                        m_Shader->setMat4(name, v);

                    else if constexpr (std::is_same_v<T, std::Ref<Texture2D>>)
                    {
                        v->Bind(textureSlot);
                        m_Shader->setInt(name, textureSlot);
                        textureSlot++;
                    }

                    else if constexpr (std::is_same_v<T, std::Ref<CubemapTexture>>)
                    {
                        v->Bind(textureSlot);
                        m_Shader->setInt(name, textureSlot);
                        textureSlot++;
                    }

                }, value);
        }

    }
}
