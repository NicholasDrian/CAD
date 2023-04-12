#version 460

layout (location = 0) in vec3 position;
//layout (location = 2) in vec3 color;

out vec3 frag_color;
out flat unsigned int frag_id;
out flat unsigned int frag_data;

layout (std140, binding = 0) uniform global
{
    mat4 view_proj;
};

layout (std140, binding = 1) uniform local
{
	mat4 model;
	vec3 color;
	unsigned int data;
	// data[0] = selectable
	// data[1] = selected
	unsigned int id;
};

layout (std140, binding = 3) readonly buffer triangleSelection
{
	unsigned int selection_buffer[];
};

void main() 
{
	gl_Position = view_proj * model * vec4(position, 1.0);

	unsigned int triangleNum = gl_VertexID / 3;
	unsigned int triangleIndex = triangleNum / 32;
	unsigned int triangleBit = 1 << (triangleNum % 32);
	bool selected = bool(data & (1 << 1)) || bool(selection_buffer[triangleIndex] & triangleBit);

	frag_color = color;
	if (selected) frag_color += vec3(0.3, 0.3, -0.3);

	frag_id = id;
	frag_data = data;

}