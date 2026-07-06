#include "Renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Logger/Logger.hpp>

#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Core/Inputs/Events.hpp>

#include "Camera.hpp"

namespace Agina {

	struct CameraBufferData {
		glm::mat4 Projection;
		glm::mat4 View;
	};

	struct RendererData {
		GLuint CameraUBO = 0;
		int WindowWidth;
		int WindowHeight;
	};

	static RendererData s_Data;

	void Renderer::Init(int width, int height)
	{

		s_Data.WindowWidth = width;
		s_Data.WindowHeight = height;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AG_CORE_ERROR("Failed To Load GLAD");
			throw std::runtime_error("Failed To Load GLAD");
		}

		AG_CORE_INFO("OpenGL Context Initialized");

		glGenBuffers(1, &s_Data.CameraUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, s_Data.CameraUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBufferData), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, s_Data.CameraUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0); 
	}

	void Renderer::ClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginScene(const Camera& cam)
	{
		CameraBufferData bufferData;
		bufferData.Projection = cam.GetProjectionMatrix(s_Data.WindowWidth, s_Data.WindowHeight);
		bufferData.View = cam.GetViewMatrix();

		glBindBuffer(GL_UNIFORM_BUFFER, s_Data.CameraUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraBufferData), &bufferData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Renderer::EndScene()
	{
		//DO LATER
	}

	void Renderer::Draw(const Mesh& mesh, Material& mat, const Transform& t) 
	{
		mat.Bind();
		mat.Set("u_Model", t.GetMatrix());
		mesh.Draw();
	}

	void Renderer::OnEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowResized>([&](WindowResized& wr)
		{
			if (wr.GetNewWidth() == 0 || wr.GetNewHeight() == 0) return;
			glViewport(0, 0, wr.GetNewWidth(), wr.GetNewHeight());
			s_Data.WindowWidth = wr.GetNewWidth();
			s_Data.WindowHeight = wr.GetNewHeight();
		});
	}

}


