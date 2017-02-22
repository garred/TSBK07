#version 150

in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D texUnit;
uniform float time;

out vec4 exColor;


void main(void)
{
	// Texture color
	exColor = texture(texUnit, vTexCoord);

	// Simulating a top-front light:
	const vec3 light_pos = vec3(1,0,0);
	float shade = dot(normalize(vNormal), light_pos);
	vec4 light_color = vec4(shade, shade, shade, 1);

	// Final color
	exColor = exColor*light_color;
}



/*
// http://glslsandbox.com/e#38557.0
vec3 dv1(vec3 v)
{
    	for(int i = 1; i <5; i++)
    	{
        	v.yzx += sin((v.zxy+vec3(0.0,2.094395102,4.188790205))*(float(i)*0.3183098862))*3.141592654;
    	}
	return v;
}

vec3 dv2(vec3 v)
{
	vec4 v4 = vec4(dv1(v),(v.x+v.y+v.z)*0.3678794412);
    	for(int i = 1; i <3; i++)
    	{
        	v.yzx += (dv1(v4.xyz)+dv1(v4.yzw)+dv1(v4.zwx)+dv1(v4.wxy)+v.zxy)*(float(i)*0.01591549431);
    	}
	return normalize(v);
}

void main( void ) {

	//vec3 pos = dv2(vec3(vTexCoord ,cos(time*0.03263098862)+sin(time*0.06183098862))*3.141592654)*100.;
	vec3 pos = dv2(vNormal)*100.;
	vec3 clr = sin(dv2(pos))*0.49+0.5;
	clr *= ((clr.x+clr.x+clr.x+clr.y+clr.z+clr.z)*0.166666);
	exColor = vec4( clr, 1.0 );

}
*/
