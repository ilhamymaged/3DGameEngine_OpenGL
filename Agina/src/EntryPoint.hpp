#pragma once
#include <Application/Application.hpp>

extern Application* Agina::CreateApplication();

int main()
{
	Application* app = Agina::CreateApplication();
	app->Run();
	delete app;
}
