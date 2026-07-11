#include "Transform.hpp"
#include <Core/Math/Math.hpp>

namespace Agina {
	
	Transform::Transform(const Vec3& pos, const Vec3& rot, const Vec3& scale)
		: Position(pos), Rotation(rot), Scale(scale){}

	Mat4 Transform::GetMatrix() const
	{
		Mat4 mat;
		mat = Math::Translate(mat, Position);
		mat = Math::Rotate(mat, Rotation);
		mat = Math::Scale(mat, Scale);
		return mat;
	}

}
