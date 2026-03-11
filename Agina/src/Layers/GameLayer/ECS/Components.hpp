#pragma once

#include <Application/Renderer/Model.hpp>
#include <Application/Renderer/SkyBox.hpp>
#include <Application/Renderer/Grid.hpp>
#include <Layers/GameLayer/Camera/Camera.hpp>
#include "Transform.hpp"

struct CameraComponent
{
	Camera camera;
};

struct MeshRenderer
{
	Model model;
};

struct SkyBoxComponent
{
	SkyBox skyBox;
};

struct GridComponent
{
	Grid grid;
};




