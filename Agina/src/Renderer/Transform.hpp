#pragma once

#include <Core/MathTypes.hpp>

namespace Agina {

	struct Transform
	{
		Transform(const Vec3& pos = Vec3(0.0f),
				  const Vec3& rot = Vec3(0.0f),
			      const Vec3& scale = Vec3(1.0f, 1.0f, 1.0f)
				 );

		Vec3 Position{ 0.0f };
		Vec3 Rotation{ 0.0f };
		Vec3 Scale{ 1.0f };

		Mat4 GetMatrix() const;
	};
}
