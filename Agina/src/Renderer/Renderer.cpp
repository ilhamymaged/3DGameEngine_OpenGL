#include "Renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Logger/Logger.hpp>

#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>

namespace Agina {
	void Renderer::Init()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AG_CORE_ERROR("Failed To Load GLAD");
			throw std::runtime_error("Failed To Load GLAD");
		}

		AG_CORE_INFO("OpenGL Context Initialized");
	}

	void Renderer::ClearColor(float r, float g, float b, float a)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(r, g, b, a);
	}

	void Renderer::Draw(const Mesh& mesh, const Material& mat) 
	{
		mat.Bind();
		mesh.Draw();
	}
}


