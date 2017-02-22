#version 150

in vec3 vertex_Normal;
in vec2 vertex_TexCoord;

uniform sampler2D texUnit;

out vec4 out_Color;

void main(void)
{
	// Texture color
	out_Color = texture(texUnit, vertex_TexCoord);

	// Simulating a top-front light:
	const vec3 light_pos = vec3(1,0,0);
	float shade = dot(normalize(vertex_Normal), light_pos);
	vec4 light_color = vec4(shade, shade, shade, 1);

	// Final color
	out_Color = out_Color*light_color;
}
