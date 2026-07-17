#version 460 core

layout (location = 0) in vec3 aPos;
layout(location= 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

layout (std140, binding = 0) uniform CameraData
{
    mat4 u_Projection;
    mat4 u_View;
    vec4 u_CameraPos;
};

out vec3 worldPos;

void main()
{
    vec3 pos = aPos + vec3(u_CameraPos.x, 0.0, u_CameraPos.z);
    worldPos = pos;
    gl_Position = u_Projection * u_View * vec4(worldPos, 1.0);
}