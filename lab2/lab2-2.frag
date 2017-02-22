#version 150

in  vec4 vertex_Normal;
in  vec2 vertex_TexCoord;

uniform sampler2D texUnit;

out vec4 out_Color;

void main(void)
{
	// Texture color
	//out_Color = texture(texUnit, vertex_Normal.xy); //Seems cool but throws errors, why?
	out_Color = texture(texUnit, vertex_TexCoord);

	// Simulating a frontal light:
	vec4 light_color = vec4(-vertex_Normal.z);

	// Final color
	out_Color = out_Color*light_color;
}
