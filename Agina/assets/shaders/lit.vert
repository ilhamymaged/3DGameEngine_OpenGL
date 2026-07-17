#version 460 core
layout(location = 0) in vec3 aPos;
layout(location= 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

layout (std140, binding = 0) uniform CameraData
{
    mat4 u_Projection;
    mat4 u_View;
    vec4 u_CameraPos;
};

layout (std140, binding = 1) uniform ShadowBuffer 
{
    mat4 u_LightSpaceMatrix;
    vec4 u_LightPos;
};

uniform mat4 u_Model;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;
out vec4 v_FragPosLightSpace; 
out mat3 v_TBN;

void main()
{
    v_FragPos = vec3(u_Model * vec4(aPos, 1.0));
    v_TexCoords = aTexCoords;
    v_FragPosLightSpace = u_LightSpaceMatrix * vec4(v_FragPos, 1.0);
    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(u_Model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    v_TBN = mat3(T, B, N); 
    v_Normal = N;          
}
