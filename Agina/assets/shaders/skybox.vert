#version 460 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform CameraBuffer 
{
    mat4 u_Projection;
    mat4 u_View;
};

out vec3 v_TexCoords;

void main() 
{
    v_TexCoords = aPos;
    
    mat4 staticView = mat4(mat3(u_View)); 
    vec4 pos = u_Projection * staticView * vec4(aPos, 1.0);
    gl_Position = pos.xyww; 
}
