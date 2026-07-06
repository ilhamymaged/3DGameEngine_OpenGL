#include "Material.hpp"

#include <Core/Logger/Logger.hpp>
#include <Core/FileSystem/FileSystem.hpp>

namespace Agina {

	Material::Material(std::shared_ptr<Shader> shader): m_Shader(shader) {}

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
		else
		{
			AG_CORE_WARN("UnSupported Built-in Material Type");
			return nullptr;
		}
	}

	std::shared_ptr<Material> Material::Lit()
	{
		AG_CORE_INFO(FileSystem::EngineAssets().string());
		static auto shader = std::make_shared<Shader>("lit",
			(FileSystem::EngineAssets() / "shaders/lit.vert").string(),
			(FileSystem::EngineAssets() / "shaders/lit.frag").string());
		return std::make_shared<Material>(shader);
	}

	std::shared_ptr<Material> Material::UnLit()
	{
		static auto shader = std::make_shared<Shader>("unlit",
			(FileSystem::EngineAssets() / "shaders/unlit.vert").string(),
			(FileSystem::EngineAssets() / "shaders/unlit.frag").string());
		return std::make_shared<Material>(shader);
	}

	void Material::Bind() const
	{
		m_Shader->Use();

		for (auto& [name, value] : m_Parameters)
		{
			std::visit([&](auto&& v)
			{
				using T = std::decay_t<decltype(v)>;

				if constexpr (std::is_same_v<T, int>)
					m_Shader->setInt(name, v);
				if constexpr (std::is_same_v<T, float>)
					m_Shader->setFloat(name, v);
				else if constexpr (std::is_same_v<T, glm::vec3>)
					m_Shader->setVec3(name, v);
				else if constexpr (std::is_same_v<T, glm::vec4>)
					m_Shader->setVec4(name, v);
				else if constexpr (std::is_same_v<T, glm::mat4>)
					m_Shader->setMat4(name, v);
			}, value);
		}
	}

}

