#pragma once

#include <ECS/Entity.hpp>
#include <imgui.h>
#include "SceneState.hpp"

namespace Agina
{
    void DrawInspector(const glm::vec2& viewPortSize, SceneState s, Entity& e,
        Scene*& scene)
    {
        ImGui::Begin("Inspector");
        if (e)
        {
            if (e.HasComponent<TagComponent>())
            {
                auto& tag = e.GetComponent<TagComponent>().tag;
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strncpy_s(buffer, tag.c_str(), sizeof(buffer));

                if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                    tag = std::string(buffer);
            }

            ImGui::Separator();
            if (e.HasComponent<Transform>())
            {
                auto& transform = e.GetComponent<Transform>();
                ImGui::DragFloat3("Translation", &transform.position.x, 0.1f);
                ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.1f);
                ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
            }

            ImGui::Separator();
            if (e.HasComponent<MeshRenderer>())
            {
                auto meshRenderer = e.GetComponent<MeshRenderer>();
                ImGui::Text("Model: %s", meshRenderer.model.GetName().c_str());
            }

            ImGui::Separator();
            if (ImGui::Button("Delete Entity")) scene->DestroyEntity(e);
        }
        else
        {

            ImGui::Text("ViewPortSize: {%0.2f,%0.2f}", viewPortSize.x, viewPortSize.y);
            const std::array<const char*, static_cast<size_t>(SceneState::Count)> StatesNames =
            {
                "EDIT",
                "PLAY",
                "PAUSE"
            };

            ImGui::Text("ViewPortMode: {%s}", StatesNames[static_cast<int>(s)]);

            ImGui::Separator();
            auto cameraView = scene->m_Registry.view<CameraComponent>();
            for (auto [entityHandle, cameraComponent] : cameraView.each())
            {
                glm::vec3 pos = cameraComponent.camera.GetPosition();
                ImGui::Text("Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
            }

            ImGui::Separator();
            ImGui::Text("No Entity Selected");
        }
        ImGui::End();
	}
}
