#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 frag_color;
out flat unsigned int frag_id;
out flat unsigned int frag_data;
out flat unsigned int frag_sub_id;

layout (std140, binding = 0) uniform global
{
    mat4 view_proj;
};

const unsigned int SELECTABLE_BIT =		1 << 0;
const unsigned int SUB_SELECTABLE_BIT = 1 << 1;
const unsigned int SELECTED_BIT =		1 << 2;
layout (std140, binding = 1) uniform local
{
	mat4 model;
	vec3 color;
	unsigned int data;
	unsigned int id;
};

layout (std140, binding = 3) readonly buffer SubSelection
{
	unsigned int selection_buffer[];
};

void main() 
{
	gl_Position = view_proj * model * vec4(position, 1.0);

	bool selected = false;
	if (data & SELECTABLE_BIT) { // If item is selectable

		selected = bool(data & SELECTED_BIT); // If item is selected, selected = true

		if (!selected && bool(data & SUB_SELECTABLE_BIT)) { // If item is not slected but a subsection might be selected
			unsigned int triangleNum = gl_VertexID / 2;
			frag_sub_id = triangleNum;
			unsigned int triangleIndex = triangleNum / 32;
			unsigned int triangleBit = 1 << (triangleNum % 32);
			selected = bool(selection_buffer[triangleIndex] & triangleBit);
		}
	}

	frag_color = color;
	if (selected) frag_color += vec3(0.5, 0.5, -0.2);
	frag_id = id;
	frag_data = data;
}