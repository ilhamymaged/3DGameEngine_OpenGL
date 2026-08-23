#pragma once

#include "Shader.hpp"
#include <variant>
#include <unordered_map>
#include <cstdint> 
#include <Core/MathTypes.hpp>
#include "Texture2D.hpp"
#include "CubemapTexture.hpp"
#include <Agina.h>

namespace Agina {

	using MaterialValue = std::variant<
		bool,
		int,
		float,
		Vec3,
		Vec4,
		Mat4,
		std::Ref<Texture2D>,
		std::Ref<CubemapTexture>
	>;

	enum MaterialType
	{
		LIT, 
		UNLIT,
		SKYBOX,
		GRID
	};

	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader, MaterialType type);
		void Bind() const;

		void Set(const std::string& name, const MaterialValue& value);
		MaterialValue& Get(const std::string& name);
		inline const MaterialType& GetMaterialType() const { return m_MaterialType; }

		static std::shared_ptr<Material> Create(MaterialType type);

		inline Shader& GetShader() {return *m_Shader;}

	private:
		std::shared_ptr<Shader> m_Shader;
		std::unordered_map<std::string, MaterialValue> m_Parameters;
		MaterialType m_MaterialType = MaterialType::UNLIT;

	private:
		static std::shared_ptr<Material> Lit();
		static std::shared_ptr<Material> UnLit();
		static std::shared_ptr<Material> SkyBox();
		static std::shared_ptr<Material> Grid();
	};
}
