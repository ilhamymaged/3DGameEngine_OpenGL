#include <EntryPoint.hpp>
#include "Game.cpp"
#include <Core/Utility/LocU.hpp>

namespace Agina
{

	static std::string s_LogoPath = (GetEngineRoot() / "logos/logo0.jpg").string(); 
	class SandBoxGame : public Application
	{
	public:
		SandBoxGame() : Application("SandBoxGame", 1280, 720, false, s_LogoPath) 
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
