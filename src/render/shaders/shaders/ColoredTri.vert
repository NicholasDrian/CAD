#version 460

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 color;
layout (location = 3) in int id;

out vec3 frag_color;
out flat int frag_id;
out flat unsigned int frag_data;

layout (std140, binding = 0) uniform global
{
    mat4 view_proj;
};

layout (std140, binding = 1) uniform local
{
	mat4 model;
	unsigned int data;
	// data[0] = selectable
	// data[1] = selected
};

void main() 
{
	gl_Position = view_proj * model * vec4(position, 1.0);
	frag_color = color;
	frag_id = id;
	frag_data = data;

}