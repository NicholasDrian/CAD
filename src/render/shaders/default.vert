#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in int id;

void main() 
{
	gl_Position = vec4(position, 1.0);
}