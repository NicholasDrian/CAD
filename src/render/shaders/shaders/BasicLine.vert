#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 tangent;

out vec3 frag_color;
out flat unsigned int frag_id;
out flat unsigned int frag_data;

layout (std140, binding = 0) uniform global
{
    mat4 view_proj;
	mat4 selected_transform;
};

layout (std140, binding = 0) readonly buffer VertSubSelection
{
	unsigned int[] vert_selection_buffer;
};

const unsigned int SELECTABLE_BIT =		1 << 0;
const unsigned int SUB_SELECTABLE_BIT = 1 << 1;
const unsigned int SELECTED_BIT =		1 << 2;
layout (std140, binding = 2) uniform local
{
	mat4 model;
	vec3 color;
	unsigned int data;
	unsigned int id;
};



void main() 
{

	uint idx = gl_VertexID / 32;
	uint bit = 1 << (gl_VertexID % 32);
	if (bool(data & SELECTED_BIT) || (bool(data & SUB_SELECTABLE_BIT) && bool(vert_selection_buffer[idx] & bit))) {
		gl_Position = view_proj * model * selected_transform * vec4(position, 1.0);
	} else {
		gl_Position = view_proj * model * vec4(position, 1.0);
	}

	frag_color = color;
	frag_id = id;
	frag_data = data;
}