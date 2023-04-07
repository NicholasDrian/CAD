#version 460

in vec3 frag_color;
in flat unsigned int frag_id;
in flat unsigned int frag_data;
	   
layout (location = 0) out vec4 out_color;
layout (location = 1) out unsigned int out_id;

void main() 
{
	out_color = vec4(frag_color, 1.0);
	if (frag_data & (1 << 0)) out_id = frag_id;
}