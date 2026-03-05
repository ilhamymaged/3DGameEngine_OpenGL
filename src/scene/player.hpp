#pragma once

#include <render/entity.hpp>
#include <core/layer.hpp>
#include <iostream>
#include <memory>
#include <core/inputs.hpp>

class Player : public Entity, public Layer
{
public:
	Player(std::shared_ptr<Model> model, const Transform& t, Input& input);

	void onEvent(std::shared_ptr<Event> e) override;
	void Update(float dt);

private:
	Input& input;
};
