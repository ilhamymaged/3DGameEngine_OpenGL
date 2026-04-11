#include <EntryPoint.hpp>
#include "Game.cpp"
#include <Core/Utility/LocU.hpp>

namespace Agina
{
	static std::string GAME_TITLE = "SandBoxGame"; 
	static uint16_t GAME_WIDTH = 1280;
	static uint16_t GAME_HEIGHT = 720;

	class SandBoxGame : public Application
	{
	public:
		SandBoxGame() : Application(GAME_TITLE, GAME_WIDTH, GAME_HEIGHT, false) 
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
