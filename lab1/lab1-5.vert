#version 150

in vec3 in_Position;
in vec4 in_Color;
uniform mat4 myMatrix;
out vec4 vertex_Color;

void main(void)
{
	gl_Position = myMatrix * vec4(in_Position, 1.0);
	vertex_Color = in_Color;
}
