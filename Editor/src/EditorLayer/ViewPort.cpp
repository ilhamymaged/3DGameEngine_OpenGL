#include "ViewPort.hpp"
#include <ECS/Components.hpp>
#include <Core/Inputs/Inputs.hpp>

namespace Agina
{
    void DrawViewPort(Scene*& activeScene, std::unique_ptr<Scene>& editorScene,
        Framebuffer* framebuffer, glm::vec2& viewportSize, SceneState& sceneState)
    {
        if (sceneState == SceneState::Play && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
            Input::Get().LockMouse();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            float windowWidth = ImGui::GetWindowSize().x;
            float buttonWidths = 150.0f;
            ImGui::SetCursorPosX((windowWidth - buttonWidths) * 0.5f);

            const char* playLabel = (sceneState == SceneState::Edit) ? " Play " : " Stop ";

            if (ImGui::Button(playLabel))
            {
                if (sceneState == SceneState::Edit)
                {
                    sceneState = SceneState::Play;
                    activeScene = Scene::Copy(editorScene.get()); 
                }
                else
                {
                    sceneState = SceneState::Edit;
                    if (activeScene != editorScene.get())
                        delete activeScene;

                    activeScene = editorScene.get(); 
                    Input::Get().UnLockMouse();
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Pause"))
                sceneState = SceneState::Pause;

            ImGui::EndMenuBar();
        }

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        if (viewportSize.x != viewportPanelSize.x || viewportSize.y != viewportPanelSize.y)
        {
            if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
            {
                framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                auto view = activeScene->m_Registry.view<CameraComponent>();
                for (auto entity : view)
                {
                    auto& cameraComp = view.get<CameraComponent>(entity);
                    cameraComp.camera.updateAspectRatio((int)viewportPanelSize.x,
                        (int)viewportPanelSize.y);
                }
            }
        }

        uint32_t textureID = framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)(uintptr_t)textureID, viewportPanelSize, { 0, 1 }, { 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();
    }
}