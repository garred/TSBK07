#version 150

in vec3 vertex_Light;
in vec2 vertex_TexCoord;

uniform sampler2D texUnit;

out vec4 out_Color;

void main(void)
{
	// Texture color
	out_Color = texture(texUnit, vertex_TexCoord);

	// Simulating a top-front light:
	vec4 light_color = vec4(vertex_Light, 1);

	// Final color
	out_Color = out_Color*light_color;
}
