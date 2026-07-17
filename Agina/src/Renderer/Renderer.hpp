#pragma once

#include <Renderer/Transform.hpp>
#include <memory>

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

		static void BeginShadowPass(const Vec3& lightPos, const Vec3& lightTarget);
		static void EndShadowPass();

		static void BeginScene(const Camera& cam);
		static void EndScene();

		static void Draw(const Mesh& mesh, Material& mat, const Transform& t = Transform());
		static void Draw(const Model& model, Material& mat, const Transform& t = Transform());
		static void DrawSkybox(const std::shared_ptr<Skybox>);
	};
}


