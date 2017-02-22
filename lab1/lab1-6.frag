#version 150

in  vec4 vertex_Color;
in  vec4 vertex_Normal;
out vec4 out_Color;

void main(void)
{
	// Simulating 3 lights: frontal white, side red, top blue.
	vec4 light_color = vec4(-vertex_Normal.z) + vec4(max(0,vertex_Normal.x)*0.5,0,0,0) + vec4(0,0,max(0,vertex_Normal.y)*0.5,0);

	out_Color = vec4(vertex_Color * light_color);
}
