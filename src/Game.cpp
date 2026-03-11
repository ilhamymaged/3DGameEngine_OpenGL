#include <Application/Application.hpp>

int main()
{
	Application* app = new Application("3D Game", 1280, 720);
	app->Run();
	delete app;
}
