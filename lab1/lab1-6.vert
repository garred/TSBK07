#version 150

in vec3 in_Position;
in vec4 in_Normal;
uniform mat4 myMatrix;
out vec4 vertex_Color;
out vec4 vertex_Normal;

void main(void)
{
	gl_Position = myMatrix * vec4(in_Position, 1.0);
	vertex_Normal = myMatrix * in_Normal;
	vertex_Color = vec4(1);
}
