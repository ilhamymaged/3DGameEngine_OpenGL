#include "Camera.hpp"
#include <Core/Inputs.hpp>
#include <Core/KeyMappings.hpp>
#include <Core/Events.hpp>
#include <UI/UI.hpp>
#include <Core/Math.hpp>

namespace Agina {

	static const Vec3 defaultFront = Vec3(0.0f, 0.0f, -1.0f);

	Camera::Camera(const Vec3& pos, float fov)
		: m_Pos(pos), m_FOV(fov), m_Front(defaultFront)
	{
		updateVectors();
	}

	Mat4 Camera::GetViewMatrix() const
	{
		if (m_Mode == CameraMode::Follow && m_FocusTarget)
		{
			Vec3 pos = *m_FocusTarget + m_FollowOffset;
			Vec3 target = *m_FocusTarget;
			return Math::LookAt(pos, target, m_WorldUp);
		}

		return Math::LookAt(m_Pos, m_Pos + m_Front, m_Up);
	}

	Mat4 Camera::GetProjectionMatrix(int width, int height) const
	{
		return Math::Perspective(m_FOV, (float)width / (float)height, 0.1f, 1000.0f);
	}

	void Camera::Update(float deltaTime)
	{
		if (m_Mode == CameraMode::Follow)
		{
			if (m_FocusTarget)
			{
				m_Pos = *m_FocusTarget + m_FollowOffset;
				m_Front = Math::Normalize(*m_FocusTarget - m_Pos);
				updateVectors();
			}
			return;
		}

		// Free camera movement via our native engine operators
		auto& input = Input::Get();
		Vec3 right = Math::Normalize(Math::Cross(m_Front, m_WorldUp));

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
					m_FirstMouse = true;
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
				double yoffset = m_LastY - ypos;

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
		Vec3 front;
		float yawRad = Math::Radians(m_Yaw);
		float pitchRad = Math::Radians(m_Pitch);

		front.x = cos(yawRad) * cos(pitchRad);
		front.y = sin(pitchRad);
		front.z = sin(yawRad) * cos(pitchRad);

		m_Front = Math::Normalize(front);
		m_Right = Math::Normalize(Math::Cross(m_Front, m_WorldUp));
		m_Up = Math::Normalize(Math::Cross(m_Right, m_Front));
	}
}
