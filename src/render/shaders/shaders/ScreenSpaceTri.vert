#version 460

layout (location = 0) in vec3 position;

out vec4 frag_color;

layout (std140, binding = 2) uniform local
{
	mat4 model;
	vec4 color;
	uint data;
	uint id;
	uint subIDOffset;
};

void main() 
{
	gl_Position = vec4(position, 1.0); 
	frag_color = color;
}