#version 150

uniform vec4 in_color;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(in_color);
}
