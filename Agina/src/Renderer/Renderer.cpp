#include "Renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Logger/Logger.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Core/Inputs/Events.hpp>
#include "Camera.hpp"
#include "Shader.hpp"
#include <Core/FileSystem/FileSystem.hpp>
#include "UniformBuffer.hpp"
#include "ShadowMapFB.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "SkyBox.hpp"

#define DEBUG_SHADOW_MAP 0

namespace Agina {

	struct CameraBufferData 
	{
		glm::mat4 Projection;
		glm::mat4 View;
	};

	struct ShadowBufferData 
	{
		glm::mat4 LightSpaceMatrix;
		glm::vec3 LightPos;
	};

	struct RendererData 
	{
		std::unique_ptr<UniformBuffer> CameraBufferUBO = nullptr;
		std::unique_ptr<UniformBuffer> ShadowBufferUBO = nullptr;
		std::unique_ptr<ShadowMapFB> ShadowDepthMap = nullptr;
		std::unique_ptr<Shader> ShadowDepthShader = nullptr;
#if DEBUG_SHADOW_MAP
		std::unique_ptr<Shader> DebugShadowMapShader = nullptr;
		std::shared_ptr<Mesh> QuadMesh;
#endif

		int WindowWidth;
		int WindowHeight;
		bool IsShadowPassActive = false;

		static constexpr uint32_t ShadowTextureSlot = 7;
	};	

	static RendererData* s_Data = nullptr;

	void Renderer::Init(int width, int height)
	{
		s_Data = new RendererData();
		s_Data->WindowWidth = width;
		s_Data->WindowHeight = height;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AG_CORE_ERROR("Failed To Load GLAD");
			throw std::runtime_error("Failed To Load GLAD");
		}

		AG_CORE_INFO("OpenGL Context Initialized");

		glEnable(GL_DEPTH_TEST);

		s_Data->CameraBufferUBO = std::make_unique<UniformBuffer>(sizeof(CameraBufferData), 0);
		s_Data->ShadowBufferUBO = std::make_unique<UniformBuffer>(sizeof(ShadowBufferData), 1);
		s_Data->ShadowDepthMap = std::make_unique<ShadowMapFB>(2048);

		s_Data->ShadowDepthShader = std::make_unique<Shader>("shadow", 
			(FileSystem::EngineAssets() / "shaders/shadow.vert").string(),
			(FileSystem::EngineAssets() / "shaders/shadow.frag").string());

#if DEBUG_SHADOW_MAP
		s_Data->DebugShadowMapShader = std::make_unique<Shader>("debugShadowMap",
			(FileSystem::EngineAssets() / "shaders/debugShadowMap.vert").string(),
			(FileSystem::EngineAssets() / "shaders/debugShadowMap.frag").string());

		s_Data->QuadMesh = Mesh::Create(MeshType::QUAD);
#endif
	}

	void Renderer::Shutdown() 
	{
		delete s_Data;
		s_Data = nullptr;
	}

	void Renderer::ClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginShadowPass(const glm::vec3& lightPos, const glm::vec3& lightTarget) 
	{
		s_Data->IsShadowPassActive = true;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 1.0f, 60.0f);
		glm::mat4 lightView = glm::lookAt(lightPos, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		ShadowBufferData shadowData{ lightProjection * lightView, lightPos };
		s_Data->ShadowBufferUBO->SetData(&shadowData, sizeof(ShadowBufferData));

		glViewport(0, 0, s_Data->ShadowDepthMap->GetResolution(), s_Data->ShadowDepthMap->GetResolution());
		s_Data->ShadowDepthMap->BindFramebuffer();
		glClear(GL_DEPTH_BUFFER_BIT);

		s_Data->ShadowDepthShader->Use();
	}

	void Renderer::EndShadowPass()
	{
		s_Data->IsShadowPassActive = false;
		s_Data->ShadowDepthMap->UnbindFramebuffer();
		glDisable(GL_CULL_FACE);
	}

	void Renderer::BeginScene(const Camera& cam)
	{
#if DEBUG_SHADOW_MAP 
		glViewport(0, 0, s_Data->WindowWidth, s_Data->WindowHeight);

		s_Data->DebugShadowMapShader->Use();
		s_Data->DebugShadowMapShader->setInt("u_ShadowMap", RendererData::ShadowTextureSlot);
		s_Data->ShadowDepthMap->BindTexture(RendererData::ShadowTextureSlot);
		s_Data->QuadMesh->Draw();
#else
		glViewport(0, 0, s_Data->WindowWidth, s_Data->WindowHeight);

		CameraBufferData cameraData
		{
			cam.GetProjectionMatrix(s_Data->WindowWidth, s_Data->WindowHeight),
			cam.GetViewMatrix()
		};

		s_Data->CameraBufferUBO->SetData(&cameraData, sizeof(CameraBufferData));
		s_Data->ShadowDepthMap->BindTexture(RendererData::ShadowTextureSlot);
#endif
	}

	void Renderer::EndScene()
	{
		//DO LATER
	}

	void Renderer::Draw(const Mesh& mesh, Material& mat, const Transform& t) 
	{

		if (s_Data->IsShadowPassActive)
		{
			s_Data->ShadowDepthShader->setMat4("u_Model", t.GetMatrix());
			mesh.Draw();
		}

		else
		{
			mat.Bind();
			mat.Set("u_Model", t.GetMatrix());
			if (mat.GetMaterialType() == MaterialType::LIT) 
				mat.Set("u_ShadowMap", static_cast<int>(RendererData::ShadowTextureSlot));
			mesh.Draw();
		}

	}

	void Renderer::DrawSkybox(const std::shared_ptr<Skybox> skybox) 
	{
		glDepthFunc(GL_LEQUAL);
		skybox->Draw();
		glDepthFunc(GL_LESS);
	}

	void Renderer::OnEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowResized>([&](WindowResized& wr)
		{
			if (wr.GetNewWidth() == 0 || wr.GetNewHeight() == 0) return;
			s_Data->WindowWidth = wr.GetNewWidth();
			s_Data->WindowHeight = wr.GetNewHeight();
			if (!s_Data->IsShadowPassActive)
				glViewport(0, 0, wr.GetNewWidth(), wr.GetNewHeight());
		});
	}

}


