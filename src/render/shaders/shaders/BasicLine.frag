#version 460

in vec3 frag_color;
in flat unsigned int frag_id;
in flat unsigned int frag_sub_id;
in flat unsigned int frag_data;
	   
layout (location = 0) out vec4 out_color;
layout (location = 1) out uvec2 out_id;

const unsigned int SELECTABLE_BIT =		1 << 0;
const unsigned int SUB_SELECTABLE_BIT = 1 << 1;
const unsigned int SELECTED_BIT =		1 << 2;

void main() 
{
	out_color = vec4(frag_color, 1.0);
	
	// why is this vector backwards!!!
	if (frag_data & SELECTABLE_BIT) out_id = uvec2(frag_sub_id, frag_id);

	//TODO
	//gl_FragDepth = gl_FragCoord.z + 0.00001;
}