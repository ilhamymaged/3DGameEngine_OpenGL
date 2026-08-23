#pragma once

#include <Renderer/Transform.hpp>
#include <Agina.h>
#include <vector>

namespace Agina {

	class Mesh;
	class Model;
	class Material;
	class Camera;
	class Event;
	class Skybox;

	class Renderer
	{
	public:
		static void Init(int width, int height);
		static void Shutdown();
		static void OnEvent(Event& e);

		static void ClearColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

		static void UpdateLightData(const Vec3& lightPos, const Vec3& lightTarget);
		static void BeginShadowPass();
		static void EndShadowPass();

		static void BeginScene(const Camera& cam);
		static void EndScene();

		static void Submit(const Mesh& mesh, Material& mat, const Transform& t = Transform());
		static void SubmitSkyBox(const std::Ref<Skybox>, Material& mat);
		static void SubmitDebugBox(const Vec3& min, const Vec3& max, 
			const Vec4& color = Vec4(0.0f, 1.0f, 0.0f, 1.0f));

		static uint32_t GetDrawCalls();

		static void SetWireFrameMode(bool enabled);
		static bool GetWireFrameMode();

		static void SetShadowsEnabled(bool enabled);
		static bool GetShadowsEnabled();

		static void SetShadowMapRendered(bool enabled);
		static bool GetShadowMapRendered();
	};
}