#include <EntryPoint.hpp>

class SandBoxGame : public Agina::Application
{
public:
	SandBoxGame() : Application("SandBoxGame", 1280, 720, false) {}

	void ShutDown() override
	{
		Application::ShutDown();
	}
};

Agina::Application* Agina::CreateApplication()
{
	return new SandBoxGame();
}

