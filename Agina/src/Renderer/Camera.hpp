#pragma once

#include <glm/glm.hpp>
#include <Core/Inputs/Events.hpp>

namespace Agina {

	enum class CameraMode
	{
		Free,
		Follow
	};

	class Camera
	{
	public:
		Camera(const glm::vec3& pos = glm::vec3(0.0f, 1.0f, 3.0f), float fov = 45.0f);

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix(int width, int height) const;

		// Runtime controls
		void Update(float deltaTime);
		void OnEvent(Event& e);

		void SetMode(CameraMode mode) { m_Mode = mode; }
		CameraMode GetMode() const { return m_Mode; }

		// Follow target - camera will position itself at target + offset and look at target
		void SetFocusTarget(const glm::vec3* target) { m_FocusTarget = target; }
		void SetFollowOffset(const glm::vec3& offset) { m_FollowOffset = offset; }

		inline const glm::vec3& GetPos() { return m_Pos; }
		inline const float& GetFOV() { return m_FOV; }

	private:
		void updateVectors();

	private:
		glm::vec3 m_Pos;
		float m_FOV;

		// orientation
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// movement params
		float m_MovementSpeed = 5.0f;
		float m_MouseSensitivity = 0.1f;
		float m_ScrollSpeed = 2.0f;

		// follow target
		CameraMode m_Mode = CameraMode::Free;
		const glm::vec3* m_FocusTarget = nullptr;
		glm::vec3 m_FollowOffset = glm::vec3(0.0f, 2.0f, 5.0f);

		// mouse tracking
		bool m_FirstMouse = true;
		double m_LastX = 0.0, m_LastY = 0.0;
	};
}

