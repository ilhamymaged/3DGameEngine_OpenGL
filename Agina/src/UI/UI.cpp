#include "UI.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Agina {
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
