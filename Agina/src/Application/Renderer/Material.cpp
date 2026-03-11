#include <Application/Renderer/Material.hpp>
#include <glad/glad.h>

Material::Material(std::shared_ptr<Shader> shader, 
	std::vector<std::shared_ptr<Texture>> textures)
	:m_Shader(shader), m_Textures(textures){}

void Material::Bind()
{
	m_Shader->Use();

	for (int i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		m_Textures[i]->Bind();
		m_Shader->setInt("diffuseTexture", i);
	}
}


