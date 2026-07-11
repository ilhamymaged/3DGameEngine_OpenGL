#pragma once

namespace Agina {

    struct Vec3 
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;

        Vec3() = default;
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        explicit Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}

        Vec3 operator+(const Vec3& o) const { return { x + o.x, y + o.y, z + o.z }; }
        Vec3 operator-(const Vec3& o) const { return { x - o.x, y - o.y, z - o.z }; }
        Vec3 operator*(float s) const { return { x * s, y * s, z * s }; }
        Vec3 operator/(float s) const { return { x / s, y / s, z / s }; }
        Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
        Vec3& operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
        bool operator<(const Vec3& o) const
        {
            if (x != o.x) return x < o.x;
            if (y != o.y) return y < o.y;
            return z < o.z;
        }
    };
    struct Vec2
    {
        float x = 0.0f, y = 0.0f;

        Vec2() = default;
        Vec2(float x, float y) : x(x), y(y) {}
        explicit Vec2(float scalar) : x(scalar), y(scalar){}

        Vec2 operator+(const Vec2& o) const { return { x + o.x, y + o.y }; }
        Vec2 operator-(const Vec2& o) const { return { x - o.x, y - o.y }; }
        Vec2 operator*(float s) const { return { x * s, y * s }; }
        Vec2 operator/(float s) const { return { x / s, y / s }; }
        Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
        Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
       
    };


    struct Vec4 
    {
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
        Vec4() = default;
        Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    };

    struct Mat4 
    {
        float data[16] = 
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        }; 

        friend Mat4 operator*(const Mat4& a, const Mat4& b);
        friend Vec4 operator*(const Mat4& m, const Vec4& v);
    };
}
