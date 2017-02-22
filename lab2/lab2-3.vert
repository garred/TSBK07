#version 150

in vec3 in_Position;
in vec4 in_Normal;
in vec2 in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 transformationMatrix;

out vec4 vertex_Normal;
out vec2 vertex_TexCoord;

void main(void)
{
	gl_Position = projectionMatrix * transformationMatrix * vec4(in_Position, 1.0);
	vertex_Normal = projectionMatrix * transformationMatrix * in_Normal;
	vertex_TexCoord = in_TexCoord;
}
