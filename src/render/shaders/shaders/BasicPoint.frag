#version 460

in vec4 frag_color;
in flat uint frag_id;
in flat uint frag_data;
in flat uint frag_sub_id;
	   
layout (location = 0) out vec4 out_color;
layout (location = 1) out uvec2 out_id;

const uint SELECTABLE_BIT =		1 << 0;
const uint SUB_SELECTABLE_BIT = 1 << 1;
const uint SELECTED_BIT =		1 << 2;

void main() 
{

	out_color = frag_color;

	// why is this vector backwards!!!
	if (frag_data & SELECTABLE_BIT) out_id = uvec2(frag_sub_id, frag_id);

	gl_FragDepth = gl_FragCoord.z - 0.00002;
}