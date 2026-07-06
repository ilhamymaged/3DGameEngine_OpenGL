#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Agina {
	
	Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
		: Position(pos), Rotation(rot), Scale(scale){}

	glm::mat4 Transform::GetMatrix() const
	{
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, Position);
		mat = glm::rotate(mat, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		mat = glm::rotate(mat, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		mat = glm::scale(mat, Scale);
		return mat;
	}

}
