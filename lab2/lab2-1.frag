#version 150

in  vec4 vertex_Color;
in  vec4 vertex_Normal;
in  vec2 vertex_TexCoord;
out vec4 out_Color;

void main(void)
{

	//out_Color = vec4(vertex_Color);
	float grid_x = (sign(fract(vertex_TexCoord.x*10.0)-0.1)+1.0)/2.0;
	float grid_y = (sign(fract(vertex_TexCoord.y*10.0)-0.1)+1.0)/2.0;
	out_Color = vec4(1 - grid_x*grid_y);

	// Simulating a frontal light:
	vec4 light_color = vec4(-vertex_Normal.z);
	out_Color = out_Color*light_color;
}
