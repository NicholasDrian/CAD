#version 460

in vec3 frag_color;
in flat int frag_id;
in flat unsigned int frag_data;
	   
layout (location = 0) out vec4 out_color;
layout (location = 1) out int out_id;

void main() 
{
	out_color = vec4(frag_color, 1.0);

	//Only write out id if selectable
	if (frag_data & (1 << 0)) out_id = frag_id;


	//TODO
	//gl_FragDepth = gl_FragCoord.z + 0.00001;
}