#version 460 core

out vec4 FragColor;

uniform sampler2D u_ShadowMap;
in vec2 TexCoords;

void main()
{
    float depthValue = texture(u_ShadowMap, TexCoords).r;
	FragColor = vec4(vec3(depthValue), TexCoords);
}

