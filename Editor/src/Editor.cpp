#include <EntryPoint.hpp>
#include "EditorLayer.hpp"

static std::string GAME_TITLE = "Editor";
static uint16_t GAME_WIDTH = 1600;
static uint16_t GAME_HEIGHT = 900;

class Editor : public Agina::Application
{
public:
	Editor() : Agina::Application(GAME_TITLE, GAME_WIDTH, GAME_HEIGHT)
	{
		m_LayerStack.PushLayer(new EditorLayer(GAME_WIDTH, GAME_HEIGHT));
	}

	void ShutDown() override { Agina::Application::ShutDown(); }
};

Agina::Application* Agina::CreateApplication()
{
	return new Editor();
}
