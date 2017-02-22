#version 150

in  vec4 vertex_Color;
uniform vec4 in_color;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(vertex_Color) + in_color;
}
