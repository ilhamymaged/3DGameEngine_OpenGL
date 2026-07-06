#pragma once

#include <glm/glm.hpp>

namespace Agina {

	struct Transform
	{
		Transform(const glm::vec3& pos = glm::vec3(0.0f), 
				  const glm::vec3& rot = glm::vec3(0.0f), 
			      const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)
				 );

		glm::vec3 Position{ 0.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		glm::mat4 GetMatrix() const;
	};
}
