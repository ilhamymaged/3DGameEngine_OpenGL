#pragma once

#include <iostream>
struct GLFWwindow;

#include <Core/MathTypes.hpp>
#include <Renderer/Framebuffer.hpp>

namespace Agina {
	
	class UI
	{
	public:

		static bool MenuItem(const std::string& label);
		static bool BeginPopup(const std::string& label);
		static void OpenPopup(const std::string& label);
		static void EndPopup();

		static bool BeginMenu(const char* label);
		static void EndMenu();

		static bool BeginPropertyGrid(const std::string& id);
		static void EndPropertyGrid();

		static void SetDarkEngineTheme();

		static bool WantsCaptureMouse();
		static bool WantsCaptureKeyboard();

		static bool BeginCombo(const char* label, const char* previewValue);
		static void EndCombo();
		static bool Selectable(const char* label, bool selected);
		static void SetItemDefaultFocus();

		static Vec2 Viewport(const std::shared_ptr<Framebuffer>& framebuffer, const std::string& name = "Viewport");

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
		static bool IsItemClicked(int i);
		static bool IsAnyItemHovered();

		static void BeginWindow(const std::string& name, bool fullScreen = false);
		static void BeginWindow(const std::string& name, const Vec2& Pos,
			const Vec2& size);
		static void EndWindow();

		static void BeginToolbar(float height = 50.0f);
		static void EndToolbar();

		static bool Checkbox(const std::string& label, bool* value);
		static bool Button(const std::string & = "Empty", float width = 0, float height = 0);

		static void Text(const std::string& label);
		static void SelectableItem(const std::string& label);

		static void DragVec3(const std::string&, Vec3&, 
			float speed = 0.1f, float min = -999.0f, float max = 999.0f, float resetValue = 0.0f);
		static void DragFloat(const std::string& label, float& value, float speed, float min, float max);

		static void CenterNextItemX(float width);
		static void CenterNextItemY(float height);
		static void Spacing(float y = 10.0f);
		static void SameLine(float offset = 0.0f);

		static void Init(GLFWwindow* window);
		static void BeginFrame();
		static void EndFrame();
		static void ShutDown();

		static void LoadFont(uint8_t index, const std::string& filepath, float size, bool setAsDefault = false);
		static void PushFont(uint8_t index);
		static void PopFont();

	};

}