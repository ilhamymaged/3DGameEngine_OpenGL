#pragma once
#include <Application/LayerStack/Layer.hpp>
#include <imgui.h>

class ImGuiLayer : public Layer
{
public:
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float dt) override;
    void OnEvent(Event& e) override;
    void OnRender() override;
};