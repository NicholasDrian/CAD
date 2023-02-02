#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in int id;

out vec3 frag_color;
out flat int frag_id;

uniform mat4 view_proj;

void main() 
{
	gl_Position = view_proj * vec4(position, 1.0);
	frag_color = color;
	frag_id = id;

}