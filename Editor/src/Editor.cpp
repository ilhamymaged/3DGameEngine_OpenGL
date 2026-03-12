#include <EntryPoint.hpp>

class Editor : public Application
{
public:
	Editor() : Application("Editor", 1600, 900)
	{

	}

	void ShutDown() override
	{
		Application::ShutDown();
	}
};

Application* Agina::CreateApplication()
{
	return new Editor();
}
