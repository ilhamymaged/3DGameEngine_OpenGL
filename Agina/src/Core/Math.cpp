#include "Math.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Agina {

    Mat4 operator*(const Mat4& a, const Mat4& b)
    {
        glm::mat4 glmA = glm::make_mat4(a.data);
        glm::mat4 glmB = glm::make_mat4(b.data);

        glm::mat4 glmResult = glmA * glmB;

        Mat4 result;
        std::memcpy(result.data, glm::value_ptr(glmResult), 16 * sizeof(float));
        return result;
    }

    Vec4 operator*(const Mat4& m, const Vec4& v)
    {
        glm::mat4 glmM = glm::make_mat4(m.data);
        glm::vec4 glmV = glm::vec4(v.x, v.y, v.z, v.w);

        glm::vec4 glmResult = glmM * glmV;

        return Vec4(glmResult.x, glmResult.y, glmResult.z, glmResult.w);
    }
}

namespace Agina::Math {

	static glm::vec3 ToGLM(const Vec3& v) 
    {
        return glm::vec3(v.x, v.y, v.z); 
    }
	
    static Vec3 ToAgina(const glm::vec3& v) 
    { 
        return Vec3(v.x, v.y, v.z); 
    }

    static glm::mat4 ToGLM(const Mat4& m) 
    {
        return glm::make_mat4(m.data); 
    }
    
    static Mat4 ToAgina(const glm::mat4& m) 
    {
        Mat4 result;
        std::memcpy(result.data, &m[0][0], 16 * sizeof(float));
        return result;
    }

    Mat4 Perspective(float fovDegrees, float aspectRatio, float zNear, float zFar) 
    {
        glm::mat4 proj = glm::perspective(glm::radians(fovDegrees), aspectRatio, zNear, zFar);
        return ToAgina(proj);
    }

    Mat4 Ortho(float left, float right, float bottom, float up, float zNear, float zFar)
    {
        glm::mat4 ortho = glm::ortho(left, right, bottom, up, zNear, zFar);
        return ToAgina(ortho);
    }

    Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up) 
    {
        return ToAgina(glm::lookAt(ToGLM(eye), ToGLM(center), ToGLM(up)));
    }

    Mat4 Translate(const Mat4& matrix, const Vec3& translation) 
    {
        return ToAgina(glm::translate(ToGLM(matrix), ToGLM(translation)));
    }

    Mat4 Scale(const Mat4& matrix, const Vec3& scale)
    {
        return ToAgina(glm::scale(ToGLM(matrix), ToGLM(scale)));
    }

    Mat4 Rotate(const Mat4& matrix, const Vec3& rotation)
    {
        glm::mat4 gmatrix = ToGLM(matrix);
        gmatrix = glm::rotate(gmatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0));
        gmatrix = glm::rotate(gmatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0));
        gmatrix = glm::rotate(gmatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0));
        return ToAgina(gmatrix);
    }

    Vec3 Normalize(const Vec3& v)
    {
        float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        return length > 0.0f ? v / length : Vec3(0.0f);
    }

    float Radians(float degrees)
    {
        return degrees * 0.0174532925f;
    }

    Vec3 Cross(const Vec3& a, const Vec3& b)
    {
        return {
               a.y * b.z - a.z * b.y,
               a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x
        };
    }

}


