#include "EditorLayer.hpp"
#include <Renderer/Renderer.hpp>

void EditorLayer::OnAttach(){}
void EditorLayer::OnDetach() {}
void EditorLayer::OnEvent(Agina::Event& e){}
void EditorLayer::OnUpdate(float dt){}
void EditorLayer::OnRender()
{
	Agina::Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}
void EditorLayer::OnUIRender() {}