#version 460 core
layout(location = 0) in vec3 aPos;
layout(location= 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

layout (std140, binding = 0) uniform CameraData
{
    mat4 u_Projection;
    mat4 u_View;
};

layout (std140, binding = 1) uniform ShadowBuffer 
{
    mat4 u_LightSpaceMatrix;
    vec3 u_LightPos;
};

uniform mat4 u_Model;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;
out vec4 v_FragPosLightSpace; 
out vec2 TexCoords;

void main()
{
    v_FragPos = vec3(u_Model * vec4(aPos, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * aNormal; 
    v_TexCoords = aTexCoords;
    v_FragPosLightSpace = u_LightSpaceMatrix * vec4(v_FragPos, 1.0);
    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}
