#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <Core/Inputs/Inputs.hpp>
#include <Core/Inputs/KeyMappings.hpp>
#include <Core/Inputs/Events.hpp>
#include <UI/UI.hpp>

namespace Agina {

	static const glm::vec3 defaultFront = glm::vec3(0.0f, 0.0f, -1.0f);

	Camera::Camera(const glm::vec3& pos, float fov)
		: m_Pos(pos), m_FOV(fov), m_Front(defaultFront)
	{
		updateVectors();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		// If following, look at target; otherwise use free orientation
		if (m_Mode == CameraMode::Follow && m_FocusTarget)
		{
			glm::vec3 pos = *m_FocusTarget + m_FollowOffset;
			glm::vec3 target = *m_FocusTarget;
			return glm::lookAt(pos, target, m_WorldUp);
		}

		return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
	}

	glm::mat4 Camera::GetProjectionMatrix(int width, int height) const
	{
		return glm::perspective(glm::radians(m_FOV), (float)width / (float)height, 0.1f, 1000.0f);
	}

	void Camera::Update(float deltaTime)
	{
		// Follow mode just positions the camera relative to the focus target
		if (m_Mode == CameraMode::Follow)
		{
			if (m_FocusTarget)
			{
				m_Pos = *m_FocusTarget + m_FollowOffset;
				// front is target - pos so GetViewMatrix can use it if needed
				m_Front = glm::normalize(*m_FocusTarget - m_Pos);
				updateVectors();
			}
			return;
		}

		// Free camera movement by keyboard
		auto& input = Input::Get();
		glm::vec3 right = glm::normalize(glm::cross(m_Front, m_WorldUp));

		if (input.IsKeyPressed(static_cast<int>(Key::W)))
			m_Pos += m_Front * m_MovementSpeed * deltaTime;
		if (input.IsKeyPressed(static_cast<int>(Key::S)))
			m_Pos -= m_Front * m_MovementSpeed * deltaTime;
		if (input.IsKeyPressed(static_cast<int>(Key::A)))
			m_Pos -= right * m_MovementSpeed * deltaTime;
		if (input.IsKeyPressed(static_cast<int>(Key::D)))
			m_Pos += right * m_MovementSpeed * deltaTime;
		if (input.IsKeyPressed(static_cast<int>(Key::Space)))
			m_Pos += m_WorldUp * m_MovementSpeed * deltaTime;
		if (input.IsKeyPressed(static_cast<int>(Key::LeftShift)))
			m_Pos -= m_WorldUp * m_MovementSpeed * deltaTime;
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseMoved>([&](MouseMoved& me)
		{
			if (Input::Get().IsCursorEnabled())
			{
				m_FirstMouse = true; // Reset look-delta so it won't snap when re-locking
				return;
			}

			if (m_Mode != CameraMode::Free) return;

			double xpos = me.getX();
			double ypos = me.getY();

			if (m_FirstMouse)
			{
				m_LastX = xpos;
				m_LastY = ypos;
				m_FirstMouse = false;
			}

			double xoffset = xpos - m_LastX;
			double yoffset = m_LastY - ypos; // reversed: y ranges bottom->top

			m_LastX = xpos;
			m_LastY = ypos;

			xoffset *= m_MouseSensitivity;
			yoffset *= m_MouseSensitivity;

			m_Yaw += (float)xoffset;
			m_Pitch += (float)yoffset;

			if (m_Pitch > 89.0f) m_Pitch = 89.0f;
			if (m_Pitch < -89.0f) m_Pitch = -89.0f;

			updateVectors();
		});

		dispatcher.Dispatch<MouseScrolled>([&](MouseScrolled& ms)
		{
			if (Input::Get().IsCursorEnabled());
			m_FOV -= (float)ms.getYoffset() * m_ScrollSpeed;
			if (m_FOV < 1.0f) m_FOV = 1.0f;
			if (m_FOV > 90.0f) m_FOV = 90.0f;
		});
	}

	void Camera::updateVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		// re-calc Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
}

