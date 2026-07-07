#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

namespace Agina {

	class UI
	{
	public:

		static bool WantsCaptureMouse();

		static void BeginWindow(const std::string& name, bool fullScreen = false);
		static void EndWindow();

		static void BeginToolbar(float height = 50.0f);
		static void EndToolbar();

		static bool Checkbox(const std::string& label, bool* value);
		static bool Button(const std::string & = "Empty", float width = 0, float height = 0);

		static void CenterNextItemX(float width);
		static void CenterNextItemY(float height);
		static void Spacing(float y = 10.0f);
		static void SameLine(float offset = 0.0f);

		static void Init(GLFWwindow* window);
		static void BeginFrame();
		static void EndFrame();
		static void ShutDown();
	};

}