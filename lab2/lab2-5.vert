#version 150

in vec3 in_Position;
in vec4 in_Normal;
in vec2 in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 lookAtMatrix;

out vec3 vertex_Light;
out vec2 vertex_TexCoord;

void main(void)
{
	mat4 model2view = lookAtMatrix * transformationMatrix;
	mat4 model2projection = projectionMatrix * model2view;

	gl_Position = model2projection * vec4(in_Position, 1.0);

	const vec3 light_pos = vec3(1,0,0);
	float shade = dot(light_pos, normalize(mat3(model2view) * vec3(in_Normal)));
	vertex_Light = vec3(shade);

	vertex_TexCoord = in_TexCoord;
}
