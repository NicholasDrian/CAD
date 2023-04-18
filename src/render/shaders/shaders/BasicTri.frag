#version 460

in vec3 frag_color;
in flat uint frag_id;
in flat uint frag_data;
in flat uint frag_sub_id_offset;
	   
layout (location = 0) out vec4 out_color;
layout (location = 1) out uvec2 out_id;

const uint SELECTABLE_BIT =		1 << 0;
const uint SUB_SELECTABLE_BIT = 1 << 1;
const uint SELECTED_BIT =		1 << 2;

layout (std140, binding = 1) readonly buffer PrimitiveSubSelection
{
	uint primitive_selection_buffer[];
};

vec3 ShiftColor(vec3 color)
{
	return color + vec3(0.5f, 0.5f, -0.5f);
}

void main() 
{
	bool selected =  bool(frag_data & SELECTED_BIT);
	if (!selected && bool(frag_data & SUB_SELECTABLE_BIT)) 
	{
		uint subID = gl_PrimitiveID + frag_sub_id_offset;
		uint index = subID / 32;
		uint bit = 1 << (subID % 32);
		selected = bool(primitive_selection_buffer[index] & bit);
	}
	if (selected) out_color = vec4(ShiftColor(frag_color), 1.0);
	else out_color = vec4(frag_color, 1.0);

	// why is this vector backwards!!!
	if (frag_data & SELECTABLE_BIT) out_id = uvec2(gl_PrimitiveID + frag_sub_id_offset, frag_id);
}