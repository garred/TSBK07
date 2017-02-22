#version 150

in vec3 vNormal;
in vec2 vTexCoord;

//uniform sampler2D texUnit;
uniform float time;

out vec4 exColor;


void main(void)
{
	// Texture color
//	exColor = texture(texUnit, vTexCoord);
	exColor = vec4(1,1,1,1);

	// Simulating a top-front light:
	const vec3 light_pos = vec3(1,0,0);
	float shade = dot(normalize(vNormal), light_pos);
	vec4 light_color = vec4(shade, shade, shade, 1);

	// Final color
	exColor = exColor*light_color;
}
