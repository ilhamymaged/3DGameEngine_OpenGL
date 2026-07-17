#include <EntryPoint.hpp>
#include "GameLayer.hpp"

namespace Agina
{
	static std::string GAME_TITLE = "SandBoxGame"; 
	static uint16_t GAME_WIDTH = 1600;
	static uint16_t GAME_HEIGHT = 900;

	class SandBoxGame : public Application
	{
	public:
		SandBoxGame() : Application(GAME_TITLE, GAME_WIDTH, GAME_HEIGHT) 
		{
			m_LayerStack.PushLayer(new GameLayer());
		}

		void ShutDown() override
		{
			Application::ShutDown();
		}
	};

	Application* CreateApplication()
	{
		return new SandBoxGame();
	}
}
