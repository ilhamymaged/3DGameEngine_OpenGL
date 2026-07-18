#pragma once

#include <iostream>
struct GLFWwindow;

#include <Core/MathTypes.hpp>
#include <Renderer/Framebuffer.hpp>

namespace Agina {

	class UI
	{
	public:

		static bool WantsCaptureMouse();

		static void Viewport(const std::shared_ptr<Framebuffer>& framebuffer, const std::string& name = "Viewport");

		static void BeginDockspace();
		static void EndDockspace();

		static bool InputText(const std::string& label, std::string& text);
		
		static bool BeginEntityNode(const std::string& label, uint32_t entityID, bool selected);
		static void EndEntityNode();

		static bool BeginComponentHeader(const std::string& name, size_t typeHashCode, bool& outRemoveComponent);
		static void EndComponentHeader();

		static bool IsWindowHovered();
		static bool IsMouseDown(int button);
		static bool IsItemClicked();

		static void BeginWindow(const std::string& name, bool fullScreen = false);
		static void EndWindow();

		static void BeginToolbar(float height = 50.0f);
		static void EndToolbar();

		static bool Checkbox(const std::string& label, bool* value);
		static bool Button(const std::string & = "Empty", float width = 0, float height = 0);

		static void Text(const std::string& label);
		static void SelectableItem(const std::string& label);

		static void DragVec3(const std::string&, Vec3&);

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