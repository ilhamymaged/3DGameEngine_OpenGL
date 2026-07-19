#pragma once

#include <Core/MathTypes.hpp>
#include <Core/Math.hpp>
#include <Core/Events.hpp>

namespace Agina {

	enum class CameraMode
	{
		Free,
		Follow
	};

	enum class CameraProjectionType
	{
		PERSPECTIVE,
		ORTHO
	};

	class Camera
	{
	public:
		Camera(const Vec3& pos = Vec3(0.0f, 4.0f, 3.0f), float fov = 45.0f);

		Mat4 GetViewMatrix() const;
		Mat4 GetProjectionMatrix(int width, int height) const;

		void Update(float deltaTime);
		void OnEvent(Event& e);

		void SetMode(CameraMode mode) { m_Mode = mode; }
		CameraMode GetMode() const { return m_Mode; }

		void SetFocusTarget(const Vec3* target) { m_FocusTarget = target; }
		void SetFollowOffset(const Vec3& offset) { m_FollowOffset = offset; }

		inline Vec3 GetPos() const { return m_Pos; }
		inline float GetFOV() const { return m_FOV; }
		inline float& GetRefFOV() { return m_FOV; }
		inline CameraProjectionType GetProjectionType() { return m_ProjType; }

		inline void SetProjectionType(CameraProjectionType type) { m_ProjType = type; }
		inline Vec3& GetRefPos() { return m_Pos; }

		inline float& GetRefPerspectiveNear() { return m_PerspectiveNear; }
		inline float& GetRefPerspectiveFar() { return m_PerspectiveFar; }

		inline float& GetRefOrthoSize() { return m_OrthoSize; }
		inline float& GetRefOrthoNear() { return m_OrthoNear; }
		inline float& GetRefOrthoFar() { return m_OrthoFar; }

	private:
		void updateVectors();

	private:
		Vec3 m_Pos;
		float m_FOV;

		CameraProjectionType m_ProjType = CameraProjectionType::PERSPECTIVE;
		float m_PerspectiveNear = 0.1f;
		float m_PerspectiveFar = 1000.0f;
		float m_OrthoSize = 10.0f;	
		float m_OrthoNear = -1.0f; 
		float m_OrthoFar = 1000.0f;

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		Vec3 m_Front = Vec3(0.0f, 0.0f, -1.0f);
		Vec3 m_Up = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 m_Right = Vec3(1.0f, 0.0f, 0.0f);
		Vec3 m_WorldUp = Vec3(0.0f, 1.0f, 0.0f);

		float m_MovementSpeed = 5.0f;
		float m_MouseSensitivity = 0.1f;
		float m_ScrollSpeed = 2.0f;

		CameraMode m_Mode = CameraMode::Free;
		const Vec3* m_FocusTarget = nullptr;
		Vec3 m_FollowOffset = Vec3(0.0f, 2.0f, 5.0f);

		bool m_FirstMouse = true;
		double m_LastX = 0.0, m_LastY = 0.0;
	};
}

