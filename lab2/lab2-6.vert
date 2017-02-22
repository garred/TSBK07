#version 150

in vec3 in_Position;
in vec4 in_Normal;
in vec2 in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 lookAtMatrix;

out vec3 vertex_Normal;
out vec2 vertex_TexCoord;

void main(void)
{
	mat4 model2view = lookAtMatrix * transformationMatrix;
	mat4 model2projection = projectionMatrix * model2view;

	gl_Position = model2projection * vec4(in_Position, 1.0);

	vertex_Normal = mat3(model2view) * vec3(in_Normal);

	vertex_TexCoord = in_TexCoord;
}
