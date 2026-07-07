#include "Renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Logger/Logger.hpp>

#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Core/Inputs/Events.hpp>

#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

#include <Core/FileSystem/FileSystem.hpp>

namespace Agina {

	struct CameraBufferData {
		glm::mat4 Projection;
		glm::mat4 View;
	};

	struct ShadowBufferData {
		glm::mat4 LightSpaceMatrix;
		glm::vec3 LightPos;
	};

	struct RendererData {
		GLuint CameraUBO = 0;
		GLuint ShadowUBO = 0;

		GLuint ShadowFBO = 0;
		GLuint ShadowMapDepthTex = 0;
		int ShadowMapResolution = 2048;
		std::unique_ptr<Shader> shadowShader = nullptr;

		int WindowWidth;
		int WindowHeight;
		bool IsShadowPassActive = false;
	};

	static RendererData s_Data;

	void Renderer::Init(int width, int height)
	{
		s_Data.ShadowMapResolution = 2048;
		s_Data.WindowWidth = width;
		s_Data.WindowHeight = height;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AG_CORE_ERROR("Failed To Load GLAD");
			throw std::runtime_error("Failed To Load GLAD");
		}

		AG_CORE_INFO("OpenGL Context Initialized");

		glEnable(GL_DEPTH_TEST);

		s_Data.shadowShader = std::make_unique<Shader>("shadow", 
			(FileSystem::EngineAssets() / "shaders/shadow.vert").string(),
			(FileSystem::EngineAssets() / "shaders/shadow.frag").string());
		
		glGenBuffers(1, &s_Data.CameraUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, s_Data.CameraUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBufferData), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, s_Data.CameraUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0); 

		glGenBuffers(1, &s_Data.ShadowUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, s_Data.ShadowUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ShadowBufferData), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, s_Data.ShadowUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glGenFramebuffers(1, &s_Data.ShadowFBO);
		glGenTextures(1, &s_Data.ShadowMapDepthTex);
		glBindTexture(GL_TEXTURE_2D, s_Data.ShadowMapDepthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			s_Data.ShadowMapResolution, s_Data.ShadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.ShadowFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s_Data.ShadowMapDepthTex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::ClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginShadowPass(const glm::vec3& lightPos, const glm::vec3& lightTarget) 
	{
		s_Data.IsShadowPassActive = true;

		glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 75.0f);
		//glm::mat4 lightProjection = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 1.0f, 40.0f);
		glm::mat4 lightView = glm::lookAt(lightPos, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		ShadowBufferData shadowData;
		shadowData.LightSpaceMatrix = lightProjection * lightView;
		shadowData.LightPos = lightPos;

		glBindBuffer(GL_UNIFORM_BUFFER, s_Data.ShadowUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShadowBufferData), &shadowData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glViewport(0, 0, s_Data.ShadowMapResolution, s_Data.ShadowMapResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.ShadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		s_Data.shadowShader->Use();
	}

	void Renderer::EndShadowPass()
	{
		s_Data.IsShadowPassActive = false;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::BeginScene(const Camera& cam)
	{
		glViewport(0, 0, s_Data.WindowWidth, s_Data.WindowHeight);

		CameraBufferData bufferData;
		bufferData.Projection = cam.GetProjectionMatrix(s_Data.WindowWidth, s_Data.WindowHeight);
		bufferData.View = cam.GetViewMatrix();

		glBindBuffer(GL_UNIFORM_BUFFER, s_Data.CameraUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraBufferData), &bufferData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, s_Data.ShadowMapDepthTex);
	}

	void Renderer::EndScene()
	{
		//DO LATER
	}

	void Renderer::Draw(const Mesh& mesh, Material& mat, const Transform& t) 
	{

		if (s_Data.IsShadowPassActive)
		{
			s_Data.shadowShader->setMat4("u_Model", t.GetMatrix());
			mesh.Draw();
		}

		else
		{
			mat.Bind();
			mat.Set("u_Model", t.GetMatrix());
			mat.Set("u_ShadowMap", 7);
			mesh.Draw();
		}

	}

	void Renderer::OnEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowResized>([&](WindowResized& wr)
		{
			if (wr.GetNewWidth() == 0 || wr.GetNewHeight() == 0) return;
			s_Data.WindowWidth = wr.GetNewWidth();
			s_Data.WindowHeight = wr.GetNewHeight();
			if (!s_Data.IsShadowPassActive)
				glViewport(0, 0, wr.GetNewWidth(), wr.GetNewHeight());
		});
	}

}


