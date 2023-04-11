#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 color;

out vec3 frag_color;
out flat unsigned int frag_id;
out flat unsigned int frag_data;

layout (std140, binding = 0) uniform global
{
    mat4 view_proj;
};

layout (std140, binding = 1) uniform local
{
	mat4 model;
	vec3 color;
	unsigned int data;
	// data[0] = selectable
	// data[1] = selected
	unsigned int id;
};

void main() 
{
	gl_Position = view_proj * model * vec4(position, 1.0);
	frag_color = color;
	if (data & (1 << 1)) frag_color += vec3(0.5, 0.5, -0.2);
	frag_id = id;
	frag_data = data;

}