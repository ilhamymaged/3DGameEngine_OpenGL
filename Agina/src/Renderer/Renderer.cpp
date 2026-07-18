#include <glad/glad.h>
#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#include <Core/Logger.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Core/Events.hpp>
#include "Camera.hpp"
#include "Shader.hpp"
#include <Core/FileSystem.hpp>
#include "UniformBuffer.hpp"
#include "ShadowMapFB.hpp"
#include "SkyBox.hpp"
#include <Renderer/Model.hpp>
#include <Core/MathTypes.hpp>

namespace Agina {

	struct RenderCommand
	{
		const Mesh* MeshPtr = nullptr;
		const Model* ModelPtr = nullptr;
		Material* MaterialPtr = nullptr;
		Mat4 Transform;
	};

	struct CameraBufferData 
	{
		Mat4 Projection;
		Mat4 View;
		Vec3 cameraPos;
		float padding;
	};

	struct ShadowBufferData 
	{
		Mat4 LightSpaceMatrix;
		Vec3 LightPos;
		float padding;
	};

	struct RendererData 
	{
		std::unique_ptr<UniformBuffer> CameraBufferUBO = nullptr;
		std::unique_ptr<UniformBuffer> ShadowBufferUBO = nullptr;
		std::unique_ptr<ShadowMapFB> ShadowDepthMap = nullptr;
		std::unique_ptr<Shader> ShadowDepthShader = nullptr;

		int WindowWidth;
		int WindowHeight;
		bool IsShadowPassActive = false;

		static constexpr uint32_t ShadowTextureSlot = 7;

		std::vector<RenderCommand> SceneQueue;
		std::vector<RenderCommand> ShadowQueue;
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
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		s_Data->CameraBufferUBO = std::make_unique<UniformBuffer>(sizeof(CameraBufferData), 0);
		s_Data->ShadowBufferUBO = std::make_unique<UniformBuffer>(sizeof(ShadowBufferData), 1);
		s_Data->ShadowDepthMap = std::make_unique<ShadowMapFB>(2048);

		s_Data->ShadowDepthShader = std::make_unique<Shader>("shadow", 
			(FileSystem::EngineAssets() / "shaders/shadow.vert").string(),
			(FileSystem::EngineAssets() / "shaders/shadow.frag").string());
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

	void Renderer::BeginShadowPass(const Vec3& lightPos, const Vec3& lightTarget) 
	{
		s_Data->IsShadowPassActive = true;

		glCullFace(GL_FRONT);

		Mat4 lightProjection = Math::Ortho(-15.0f, 15.0f, -15.0f, 15.0f, 1.0f, 60.0f);
		Mat4 lightView = Math::LookAt(lightPos, lightTarget, Vec3(0.0f, 1.0f, 0.0f));

		ShadowBufferData shadowData{ lightProjection * lightView, lightPos, 0.0f };
		s_Data->ShadowBufferUBO->SetData(&shadowData, sizeof(ShadowBufferData));

		glViewport(0, 0, s_Data->ShadowDepthMap->GetResolution(), s_Data->ShadowDepthMap->GetResolution());
		s_Data->ShadowDepthMap->BindFramebuffer();
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::EndShadowPass()
	{
		s_Data->ShadowDepthShader->Use();
		for (const auto& cmd : s_Data->ShadowQueue)
		{
			s_Data->ShadowDepthShader->setMat4("u_Model", cmd.Transform);
			if (cmd.MeshPtr) cmd.MeshPtr->Draw();
			if (cmd.ModelPtr) cmd.ModelPtr->Draw();
		}

		s_Data->ShadowQueue.clear();	
		s_Data->IsShadowPassActive = false;
		s_Data->ShadowDepthMap->UnbindFramebuffer();
		glCullFace(GL_BACK);
	}

	void Renderer::BeginScene(const Camera& cam)
	{
		glViewport(0, 0, s_Data->WindowWidth, s_Data->WindowHeight);

		CameraBufferData cameraData
		{
			cam.GetProjectionMatrix(s_Data->WindowWidth, s_Data->WindowHeight),
			cam.GetViewMatrix(),
			cam.GetPos(),
			0.0f
		};

		s_Data->CameraBufferUBO->SetData(&cameraData, sizeof(CameraBufferData));
		s_Data->ShadowDepthMap->BindTexture(RendererData::ShadowTextureSlot);
	}

	void Renderer::EndScene()
	{
		//Sorting By Material To Minimize State Changes
		std::sort(s_Data->SceneQueue.begin(), s_Data->SceneQueue.end(),
			[](const RenderCommand& a, const RenderCommand& b) {
				return a.MaterialPtr < b.MaterialPtr;
			});

		Material* activeMaterial = nullptr;

		for (const auto& cmd : s_Data->SceneQueue)
		{
			if (cmd.MaterialPtr != activeMaterial)
			{
				activeMaterial = cmd.MaterialPtr;
				activeMaterial->Bind();

				if (activeMaterial->GetMaterialType() == MaterialType::LIT)
					activeMaterial->Set("u_ShadowMap", static_cast<int>(RendererData::ShadowTextureSlot));
			}

			if (activeMaterial->GetMaterialType() != MaterialType::GRID)
				activeMaterial->Set("u_Model", cmd.Transform);

			if (cmd.MeshPtr) cmd.MeshPtr->Draw();
			if (cmd.ModelPtr) cmd.ModelPtr->Draw();
		}

		s_Data->SceneQueue.clear();
	}

	void Renderer::Draw(const Mesh& mesh, Material& mat, const Transform& t) 
	{
		RenderCommand cmd{ &mesh, nullptr, &mat, t.GetMatrix() };
		if (s_Data->IsShadowPassActive) s_Data->ShadowQueue.push_back(cmd);
		else s_Data->SceneQueue.push_back(cmd);
	}

	void Renderer::Draw(const Model& model, Material& mat, const Transform& t)
	{
		RenderCommand cmd{ nullptr, &model, &mat, t.GetMatrix() };
		if (s_Data->IsShadowPassActive) s_Data->ShadowQueue.push_back(cmd);
		else s_Data->SceneQueue.push_back(cmd);
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


