#version 460 core

out vec4 FragColor;

uniform vec3 u_Color;

in vec2 TexCoords;
uniform sampler2D u_Texture;

uniform bool u_HasColor;

void main() 
{
	if(u_HasColor) FragColor = vec4(u_Color, 1.0);
	else FragColor = texture(u_Texture, TexCoords);
}

