#pragma once

#include <Renderer/Model.hpp>
#include <Renderer/SkyBox.hpp>
#include <Renderer/Grid.hpp>
#include <Systems/Camera/Camera.hpp>
#include "Transform.hpp"

namespace Agina
{
	struct TagComponent
	{
		std::string tag;
		TagComponent(const std::string& t) : tag(t) {}
	};

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
}

