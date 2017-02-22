#version 150

in  vec4 vertex_Color;
in  vec4 vertex_Normal;
in  vec3 vertex_TexCoord;
out vec4 out_Color;

void main(void)
{
	vec3 v = vertex_TexCoord;
	out_Color = vec4(sin(v.x*20)+1, cos(v.y*20)+1, sin(v.z*20)+1, 1.0);

	// Simulating a frontal light:
	vec4 light_color = vec4(-vertex_Normal.z);
	out_Color = out_Color*light_color;
}
