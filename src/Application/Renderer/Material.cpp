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

	//if (m_Textures.size() >= 1) { glActiveTexture(GL_TEXTURE0); m_Textures[0]->Bind(); m_Shader->setInt("albedoMap", 0); }
	//if (m_Textures.size() >= 2) { glActiveTexture(GL_TEXTURE1); m_Textures[1]->Bind(); m_Shader->setInt("normalMap", 1); }
	//if (m_Textures.size() >= 3) { glActiveTexture(GL_TEXTURE2); m_Textures[2]->Bind(); m_Shader->setInt("metallicMap", 2); }
	//if (m_Textures.size() >= 4) { glActiveTexture(GL_TEXTURE3); m_Textures[3]->Bind(); m_Shader->setInt("roughnessMap", 3); }
	//if (m_Textures.size() >= 5) { glActiveTexture(GL_TEXTURE4); m_Textures[4]->Bind(); m_Shader->setInt("aoMap", 4); }
}


