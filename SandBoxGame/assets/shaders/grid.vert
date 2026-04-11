#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 worldPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main()
{
    vec3 pos = aPos + vec3(cameraPos.x, 0.0, cameraPos.z);
    worldPos = pos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}