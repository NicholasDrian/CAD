#version 460

in vec3 frag_color;
in flat int frag_id;
	   
layout (location = 0) out vec4 out_color;
layout (location = 1) out int out_id;

void main() 
{
	out_color = vec4(frag_color, 1.0);
	out_id = frag_id;
}