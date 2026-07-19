#include "UI.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <imgui_internal.h>
#include <Core/Logger.hpp>

namespace Agina {

	bool UI::BeginPropertyGrid(const std::string& id)
	{
		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings;
		if (ImGui::BeginTable(id.c_str(), 2, tableFlags))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
			return true;
		}
		return false;
	}

	void UI::EndPropertyGrid()
	{
		ImGui::EndTable();
	}

	void UI::SetDarkEngineTheme()
	{
		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		// Window & Panel Configuration
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(6.0f, 4.0f);
		style.WindowRounding = 6.0f;
		style.FrameRounding = 4.0f;
		style.PopupRounding = 4.0f;
		style.GrabRounding = 3.0f;
		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;

		// Color Palette (Deep Obsidian & Charcoal with Subtle Slate Accents)
		colors[ImGuiCol_Text] = ImVec4(0.85f, 0.87f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.45f, 0.48f, 0.52f, 1.00f);

		colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f); // Main panels
		colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);

		colors[ImGuiCol_Border] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f); // Subtle separators
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.16f, 1.00f); // Input boxes
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.19f, 0.23f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);

		colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);

		colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);

		// Buttons & Toggles
		colors[ImGuiCol_Button] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.28f, 0.30f, 0.35f, 1.00f);

		// Headers (Tree nodes, collapsing headers)
		colors[ImGuiCol_Header] = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.19f, 0.23f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.26f, 0.31f, 1.00f);

		// Docking & Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.19f, 0.23f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.12f, 0.44f, 0.41f, 0.40f); // Muted teal selection
	}

	struct UIStorage
	{
		std::unordered_map<uint8_t, ImFont*> FontRegistry;
	};

	static UIStorage* s_Data = nullptr;

	bool UI::BeginCombo(const char* label, const char* previewValue) { return ImGui::BeginCombo(label, previewValue); }
	void UI::EndCombo() { ImGui::EndCombo(); }
	bool UI::Selectable(const char* label, bool selected) { return ImGui::Selectable(label, selected); }
	void UI::SetItemDefaultFocus() { ImGui::SetItemDefaultFocus(); }

	bool UI::MenuItem(const std::string& label)
	{
		return ImGui::MenuItem(label.c_str());
	}

	bool UI::BeginPopup(const std::string& label)
	{
		return ImGui::BeginPopup(label.c_str());
	}

	void UI::OpenPopup(const std::string& label) { ImGui::OpenPopup(label.c_str()); }
	void UI::EndPopup() { ImGui::EndPopup(); }
	
	void UI::BeginDockspace()
	{
		static bool dockspaceOpen = true;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		float toolbarHeight = 0.0f; // Hard-codded
		ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + toolbarHeight));
		ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - toolbarHeight));
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("AginaEditorWorkspace", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar(3); 

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceId = ImGui::GetID("AginaCentralDockSpace");

			if (ImGui::DockBuilderGetNode(dockspaceId) == nullptr)
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImVec2(viewport->WorkSize.x, 
					viewport->WorkSize.y - toolbarHeight));

				ImGuiID dockMainId = dockspaceId; // This acts as the center workspace (Viewport)
				ImGuiID dockLeftId, dockLeftBottomId, dockRightId, dockBottomId;

				// 1. Slice Left Sidebar out from Main (20% of total width) -> Holds Scene Hierarchy
				dockLeftId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Left, 0.20f, nullptr, &dockMainId);

				// 2. Slice Bottom Left out from the Left Sidebar (50% of left height) -> Holds Properties
				dockLeftBottomId = ImGui::DockBuilderSplitNode(dockLeftId, ImGuiDir_Down, 0.50f, nullptr, &dockLeftId);

				// 3. Slice Bottom out from the central area (25% of center height) -> Holds Content Browser
				dockBottomId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Down, 0.25f, nullptr, &dockMainId);

				// 4. Slice Right Sidebar out from the remaining center space (25% width) -> Holds Settings
				dockRightId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.25f, nullptr, &dockMainId);

				// 5. Match your ImGui::Begin window titles exactly to their locked slots
				ImGui::DockBuilderDockWindow("Scene Hierarchy", dockLeftId);
				ImGui::DockBuilderDockWindow("Properties", dockLeftBottomId);
				ImGui::DockBuilderDockWindow("Content Browser", dockBottomId);
				ImGui::DockBuilderDockWindow("Settings", dockRightId);
				ImGui::DockBuilderDockWindow("Viewport", dockMainId); // Gets whatever is left in the center

				ImGui::DockBuilderFinish(dockspaceId);
			}

			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		}
	}

	void UI::EndDockspace()
	{
		ImGui::End(); 
	}

	bool UI::InputText(const std::string& label, std::string& text)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, text.c_str(), sizeof(buffer) - 1);

		if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer)))
		{
			text = std::string(buffer);
			return true;
		}
		return false;
	}

	bool UI::BeginEntityNode(const std::string& label, uint32_t entityID, bool selected)
	{
		ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		return ImGui::TreeNodeEx((void*)(uint64_t)entityID, flags, "%s", label.c_str());
	}

	void UI::EndEntityNode()
	{
		ImGui::TreePop();
	}

	bool UI::BeginComponentHeader(const std::string& name, size_t typeHashCode, bool& outRemoveComponent)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | 
			ImGuiTreeNodeFlags_SpanAvailWidth |  ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImGui::Separator();

		UI::PushFont(1);
		bool open = ImGui::TreeNodeEx((void*)typeHashCode, treeNodeFlags, "%s", name.c_str());
		UI::PopFont();

		ImGui::PopStyleVar();

		ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		outRemoveComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				outRemoveComponent = true;

			ImGui::EndPopup();
		}

		return open;
	}

	void UI::DragFloat(const std::string& label, float& value, float speed, float min, float max)
	{
		ImGui::DragFloat(label.c_str(), &value, speed, min, max);
	}

	void UI::EndComponentHeader()
	{
		ImGui::TreePop();
	}

	bool UI::IsWindowHovered() { return ImGui::IsWindowHovered(); }
	bool UI::IsMouseDown(int button) { return ImGui::IsMouseDown(button); }
	bool UI::IsItemClicked() { return ImGui::IsItemClicked(); }

	bool UI::WantsCaptureMouse()
	{
		return ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureMouse;
	}

	bool UI::WantsCaptureKeyboard()
	{
		return ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureKeyboard;
	}

	void UI::BeginWindow(const std::string& name, bool fullScreen)
	{
		if (fullScreen)
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

			ImGui::Begin(name.c_str(), nullptr,
				ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize);
		}
		else ImGui::Begin(name.c_str());
	}

	void UI::BeginWindow(const std::string& name, const Vec2& Pos,
		const Vec2& size)
	{
		ImGui::SetNextWindowPos(ImVec2(Pos.x, Pos.y));
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y));

		ImGui::Begin(name.c_str(), nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize);
	}

	void UI::EndWindow()
	{
		ImGui::End();
	}

	Vec2 UI::Viewport(const std::shared_ptr<Framebuffer>& framebuffer, const std::string& name)
	{
		if (!framebuffer) return { 0.0f, 0.0f };

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(name.c_str());
		
		ImGui::PopStyleVar();

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		const auto& spec = framebuffer->GetSpecification();

		uint64_t textureID = framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((ImTextureID)textureID, panelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		return Vec2(panelSize.x, panelSize.y);
	}

	void UI::BeginToolbar(float height)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, height));
		
		ImGui::Begin("##Toolbar", nullptr,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoSavedSettings);
	}

	void UI::SelectableItem(const std::string& label)
	{
		ImGui::Selectable(label.c_str());
	}

	void UI::Text(const std::string& label)
	{
		ImGui::Text(label.c_str());
	}

	void UI::DragVec3(const std::string& label, Vec3& vec3, float speed, float min, float max, float resetValue)
	{
		ImGui::PushID(label.c_str());

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(label.c_str());

		ImGui::TableSetColumnIndex(1);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 1.0f, lineHeight };
		float itemWidth = (ImGui::GetContentRegionAvail().x - (buttonSize.x * 3.0f)) / 3.0f;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.75f, 0.20f, 0.20f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.85f, 0.30f, 0.30f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.75f, 0.20f, 0.20f, 1.00f });
		if (ImGui::Button("X", buttonSize)) vec3.x = resetValue; // Reset on click
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##X", &vec3.x, speed, min, max, "%.2f");
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.20f, 0.65f, 0.20f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.30f, 0.75f, 0.30f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.20f, 0.65f, 0.20f, 1.00f });
		if (ImGui::Button("Y", buttonSize)) vec3.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##Y", &vec3.y, speed, min, max, "%.2f");
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.20f, 0.20f, 0.75f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.30f, 0.30f, 0.85f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.20f, 0.20f, 0.75f, 1.00f });
		if (ImGui::Button("Z", buttonSize)) vec3.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##Z", &vec3.z, speed, min, max, "%.2f");

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	void UI::EndToolbar()
	{
		ImGui::End();
	}

	bool UI::Checkbox(const std::string& label, bool* value)
	{
		return ImGui::Checkbox(label.c_str(), value);
	}

	bool UI::Button(const std::string& name, float width, float height)
	{
		return ImGui::Button(name.c_str(), ImVec2(width, height));
	}

	void UI::CenterNextItemX(float width)
	{
		float avail = ImGui::GetContentRegionAvail().x;
		float offset = (avail - width) * 0.5;

		if (offset > 0.0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
	}

	void UI::CenterNextItemY(float height)
	{
		float avail = ImGui::GetContentRegionAvail().y;
		float offset = (avail - height) * 0.5;

		if (offset > 0.0) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset);
	}

	void UI::Spacing(float y)
	{
		ImGui::Dummy(ImVec2(0.0f, y));
	}

	void UI::SameLine(float offset)
	{
		ImGui::SameLine(offset);
	}

	void UI::Init(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		s_Data = new UIStorage();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 
	}

	void UI::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void UI::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void UI::ShutDown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		delete s_Data;
		s_Data = nullptr;
	}

	void UI::LoadFont(uint8_t index, const std::string& filepath, float size, bool setAsDefault)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), size);

		if (font)
		{
			s_Data->FontRegistry[index] = font;

			if (setAsDefault)
			{
				io.FontDefault = font;
			}

			// Force the OpenGL backend to refresh and rebuild the font texture atlas on the GPU
			ImGui_ImplOpenGL3_DestroyDeviceObjects();
			ImGui_ImplOpenGL3_CreateDeviceObjects();
		}
		else
		{
			AG_CORE_ERROR("UI::LoadFont -> Failed to load font '{0}' from: {1}", index, filepath);
		}
	}

	void UI::PushFont(uint8_t index)
	{
		auto it = s_Data->FontRegistry.find(index);
		if (it != s_Data->FontRegistry.end())
		{
			ImGui::PushFont(it->second);
		}
		else
		{
			AG_CORE_WARN("UI::PushFont -> Font '{0}' not found in registry!", index);
		}
	}

	void UI::PopFont()
	{
		ImGui::PopFont();
	}
}
