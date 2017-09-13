#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

out vec2 texCoord;
out vec3 vNormal;
out vec3 vPos;



void main(void)
{
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);

	vNormal = normalize(mat3(mdlMatrix) * inNormal);
	texCoord = inTexCoord;
	vPos = vec3(mdlMatrix * vec4(inPosition, 1.0));
}
