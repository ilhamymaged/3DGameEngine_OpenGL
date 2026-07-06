#pragma once

#include <Renderer/Transform.hpp>

namespace Agina {

	class Mesh;
	class Material;
	class Camera;
	class Event;

	class Renderer 
	{
	public:
		static void Init(int width, int height);
		static void OnEvent(Event& e);
		static void ClearColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
		static void BeginScene(const Camera& cam);
		static void EndScene();
		static void Draw(const Mesh& mesh, Material& mat, const Transform& t = Transform());
	};
}


