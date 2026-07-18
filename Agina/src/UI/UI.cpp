#include "UI.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

namespace Agina {

	void UI::BeginDockspace()
	{
		static bool dockspaceOpen = true;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

		// Grab the main viewport size and position to span the entire screen surface
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		// Strip all window decorations and borders for a seamless background canvas
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// Remove inner padding so docked views touch the screen edges completely
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("AginaEditorWorkspace", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar(3); // Pop rounding, border size, and window padding

		// Submit the central dock identifier node
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceId = ImGui::GetID("AginaCentralDockSpace");
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		}
	}

	void UI::EndDockspace()
	{
		ImGui::End(); // Closes the master background workspace window
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
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |  ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		//float lineHeight = ImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		float lineHeight = 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeHashCode, treeNodeFlags, "%s", name.c_str());
		ImGui::PopStyleVar();

		// Position the "+" or gear button at the right edge
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

	void UI::EndWindow()
	{
		ImGui::End();
	}

	void UI::Viewport(const std::shared_ptr<Framebuffer>& framebuffer, const std::string& name)
	{
		if (!framebuffer) return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(name.c_str());
		ImGui::PopStyleVar();

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		const auto& spec = framebuffer->GetSpecification();

		if (panelSize.x > 0.0f && panelSize.y > 0.0f && 
			(spec.Width != (uint32_t)panelSize.x || spec.Height != (uint32_t)panelSize.y))
		{
			framebuffer->Resize((uint32_t)panelSize.x, (uint32_t)panelSize.y);
		}

		uint64_t textureID = framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((ImTextureID)textureID, panelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
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

	void UI::DragVec3(const std::string& label, Vec3& vec3)
	{
		ImGui::DragFloat3(label.c_str(), &vec3.x, 0.1f);
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
	}
}
