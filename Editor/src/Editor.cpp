#include <EntryPoint.hpp>
#include "EditorLayer.hpp"

class Editor : public Agina::Application
{
public:
	Editor() : Agina::Application("Agina Editor", 1280, 720) 
	{
		m_LayerStack.PushLayer(new EditorLayer());
	}

	void ShutDown() override { Agina::Application::ShutDown(); }
};

Agina::Application* Agina::CreateApplication()
{
	return new Editor();
}
