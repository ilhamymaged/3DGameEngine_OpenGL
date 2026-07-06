#pragma once

namespace Agina {

	class Mesh;
	class Material;

	class Renderer 
	{
	public:
		static void Init();
		static void ClearColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
		static void Draw(const Mesh& mesh, const Material& mat);
	};
}


