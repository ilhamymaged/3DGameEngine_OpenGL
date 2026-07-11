#pragma once
#include "MathTypes.hpp"

namespace Agina::Math {
    Mat4 Perspective(float fovDegrees, float aspectRatio, float zNear, float zFar);
    Mat4 Ortho(float left, float right, float bottom, float up, float zNear, float zFar);
    Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    Mat4 Translate(const Mat4& matrix, const Vec3& translation);
    Mat4 Scale(const Mat4& matrix, const Vec3& scale);
    Mat4 Rotate(const Mat4& matrix, const Vec3& rotation);
    Vec3 Normalize(const Vec3& vec);
    float Radians(float degrees);
    Vec3 Cross(const Vec3& a, const Vec3& b);
}
