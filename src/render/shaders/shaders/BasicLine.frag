#version 460

in vec3 frag_color;
in flat unsigned int frag_id;
in flat unsigned int frag_data;
	   
layout (location = 0) out vec4 out_color;
layout (location = 1) out uvec2 out_id;

const unsigned int SELECTABLE_BIT =		1 << 0;
const unsigned int SUB_SELECTABLE_BIT = 1 << 1;
const unsigned int SELECTED_BIT =		1 << 2;

layout (std140, binding = 3) readonly buffer PrimitiveSubSelection
{
	unsigned int primitive_selection_buffer[];
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
		uint index = gl_PrimitiveID / 32;
		uint bit = 1 << (gl_PrimitiveID % 32);
		selected = bool(primitive_selection_buffer[index] & bit);
	}
	if (selected) out_color = vec4(ShiftColor(frag_color), 1.0);
	else out_color = vec4(frag_color, 1.0);
	
	// why is this vector backwards!!!
	if (frag_data & SELECTABLE_BIT) out_id = uvec2(gl_PrimitiveID, frag_id);

	gl_FragDepth = gl_FragCoord.z - 0.00001;
}