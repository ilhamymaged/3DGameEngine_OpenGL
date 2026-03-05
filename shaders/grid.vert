#version 460 core

const vec3 Pos[4] = vec3[4](
	vec3(-1.0f, 0.0f, -1.0f), 
	vec3(1.0f, 0.0f,  -1.0f), 
	vec3(1.0f, 0.0f,   1.0f), 
	vec3(-1.0f, 0.0f,  1.0f) 
);

const int indices[6] = int[6](0, 2, 1, 2, 0, 3);

uniform mat4 view;
uniform mat4 proj;
uniform vec3 cameraPos;

void main()
{
	int index = indices[gl_VertexID];
	vec4 vPos = vec4(Pos[index], 1.0f);

	vPos.x += cameraPos.x;
	vPos.z += cameraPos.z;

	gl_Position  = proj * view * vPos;
}

