#version 150

in vec3 in_Position;
in vec4 in_Normal;
in vec2 in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 lookAtMatrix;

out vec4 vertex_Normal;
out vec2 vertex_TexCoord;

void main(void)
{
	mat4 world2view = projectionMatrix * lookAtMatrix * transformationMatrix;
	gl_Position = world2view * vec4(in_Position, 1.0);
	vertex_Normal = world2view * in_Normal;
	vertex_TexCoord = in_TexCoord;
}
