#version 150

in vec3 inPosition;
in vec4 inNormal;
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 lookAtMatrix;
uniform float time;

out vec3 vNormal;
out vec2 vTexCoord;

void main(void)
{
	mat4 model2projection = projectionMatrix * lookAtMatrix * transformationMatrix;

	vec3 pos = inPosition;
	pos.y = pos.y + sin(pos.y*3 + time*5)*0.1;
	gl_Position = model2projection * vec4(pos, 1.0);

	vNormal = mat3(transformationMatrix) * vec3(inNormal);

	vTexCoord = inTexCoord;
}
