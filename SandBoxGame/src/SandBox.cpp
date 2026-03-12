#include <Application/Application.hpp>

int main()
{
	Application* app = new Application("Editor", 1600, 900);
	app->Run();
	delete app;
}
