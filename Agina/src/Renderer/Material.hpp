#pragma once

#include "Shader.hpp"
#include <memory>
#include <variant>
#include <glm/glm.hpp>
#include <unordered_map>
#include "Texture.hpp"

namespace Agina {

	using MaterialValue = std::variant<int, float, glm::vec3, glm::vec4, glm::mat4>;

	enum MaterialType
	{
		LIT, 
		UNLIT 
	};

	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader);
		void Bind() const;
		
		void Set(const std::string& name, const MaterialValue& value);
		const MaterialValue& Get(const std::string& name) const;

		static std::shared_ptr<Material> Create(MaterialType type);

	private:
		std::shared_ptr<Shader> m_Shader;
		std::unordered_map<std::string, MaterialValue> m_Parameters;

	private:
		static std::shared_ptr<Material> Lit();
		static std::shared_ptr<Material> UnLit();
	};
}
