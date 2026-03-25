#pragma once
#include <Application.hpp>

extern Agina::Application* Agina::CreateApplication();

int main()
{
	Agina::Application* app = Agina::CreateApplication();
	app->Run();
    app->ShutDown();
	delete app;
}
