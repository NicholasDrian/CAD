#version 460

layout (location = 0) in vec3 position;

out vec4 frag_color;
out flat uint frag_id;
out flat uint frag_data;
out flat uint frag_sub_id;

layout (std140, binding = 0) uniform global
{
    mat4 view_proj;
	mat4 selected_transform;
};

layout (std140, binding = 0) readonly buffer VertSubSelection
{
	uint[] vert_selection_buffer;
};

const uint SELECTABLE_BIT =		1 << 0;
const uint SUB_SELECTABLE_BIT = 1 << 1;
const uint SELECTED_BIT =		1 << 2;
layout (std140, binding = 2) uniform local
{
	mat4 model;
	vec4 color;
	uint data;
	uint id;
};

vec4 ShiftColor(vec4 color)
{
	return color + vec4(0.5f, 0.5f, -0.5f, 1.0f);
}

void main() 
{
	uint idx = gl_VertexID / 32;
	uint bit = 1 << (gl_VertexID % 32);
	if (bool(data & SELECTED_BIT) || (bool(data & SUB_SELECTABLE_BIT) && bool(vert_selection_buffer[idx] & bit))) {
		gl_Position = view_proj * selected_transform * model * vec4(position, 1.0);
		frag_color = ShiftColor(color);
	} else {
		gl_Position = view_proj * model * vec4(position, 1.0);
		frag_color = color;
	}

	frag_id = id;
	frag_data = data;
	frag_sub_id = gl_VertexID;
}