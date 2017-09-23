#version 150

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vPos;

// Material related
#define N_TEXTURES 8
uniform sampler2D colorTexUnit[N_TEXTURES];
uniform sampler2D alphaTexUnit[N_TEXTURES];
uniform float tilesTexUnit[N_TEXTURES];
uniform float specularExponent;
uniform float k_d;
uniform float transparency;

// Time
uniform float time;

// lights
#define N_LIGHTS 8
uniform bool useLights;
uniform bool isActive[N_LIGHTS];
uniform vec3 lightSourcesDirPosArr[N_LIGHTS];
uniform vec3 lightSourcesColorArr[N_LIGHTS];
uniform float lightRange[N_LIGHTS];
uniform float ambientFactor[N_LIGHTS];
uniform bool isDirectional[N_LIGHTS];
uniform vec3 cameraWorldPos;

#define N_LEVELS 2.0

out vec4 exColor;


vec3 toonize(vec3 v) {
	float exponent = 100.0 / pow(length(vPos-cameraWorldPos), 0.4);
	exponent = clamp(exponent, 1, 20);

	float v_int, v_float;
	v_int = (floor(length(v)*N_LEVELS)/N_LEVELS);
	v_float = (length(v) - v_int);
	v_float = pow(v_float * N_LEVELS, exponent) / N_LEVELS;
	v = v * (v_int + v_float);
	return v;
}


void main(void)
{
	// Texture color
	vec4 t_color = texture(colorTexUnit[0], vTexCoord*tilesTexUnit[0]) * texture(alphaTexUnit[0], vTexCoord);
	t_color 		+= texture(colorTexUnit[1], vTexCoord*tilesTexUnit[1]) * texture(alphaTexUnit[1], vTexCoord);
	t_color 		+= texture(colorTexUnit[2], vTexCoord*tilesTexUnit[2]) * texture(alphaTexUnit[2], vTexCoord);
	t_color 		+= texture(colorTexUnit[3], vTexCoord*tilesTexUnit[3]) * texture(alphaTexUnit[3], vTexCoord);
	t_color 		+= texture(colorTexUnit[4], vTexCoord*tilesTexUnit[4]) * texture(alphaTexUnit[4], vTexCoord);
	t_color 		+= texture(colorTexUnit[5], vTexCoord*tilesTexUnit[5]) * texture(alphaTexUnit[5], vTexCoord);
	t_color 		+= texture(colorTexUnit[6], vTexCoord*tilesTexUnit[6]) * texture(alphaTexUnit[6], vTexCoord);
	t_color 		+= texture(colorTexUnit[7], vTexCoord*tilesTexUnit[7]) * texture(alphaTexUnit[7], vTexCoord);

    vec3 l;

    if (useLights) {
        vec3 i_amb = vec3(0.0);
        vec3 i_diff = vec3(0.0);
        vec3 i_spec = vec3(0.0);

        for (int i=0; i<N_LIGHTS; i++) {
            if (isActive[i]) {
                vec3 l_dir;
                float l_d;
                if (isDirectional[i]) {
                    l_dir = normalize(lightSourcesDirPosArr[i]);
                    l_d = 1.0;
                } else {
                    vec3 l_dif = vPos - lightSourcesDirPosArr[i];
                    l_dir = normalize(l_dif);
                    l_d = max(0, lightRange[i]-length(l_dif)) / lightRange[i];
                    l_d = l_d*l_d;
                }

                // Ambient light component
                i_amb += k_d * ambientFactor[i] * lightSourcesColorArr[i];

                // Diffuse light component
                i_diff += k_d * max(0, -dot(vNormal, l_dir)) * lightSourcesColorArr[i] * l_d;

                // Specular light component
                if (specularExponent > 0) {
                    vec3 r = reflect(l_dir, vNormal);
                    vec3 v = normalize(cameraWorldPos - vPos);
                    i_spec += k_d * pow(max(0, dot(r,v)), specularExponent) * lightSourcesColorArr[i];
                }
            }
        }
        l = i_amb + i_diff + i_spec;
        l = toonize(l);
        l = clamp(l, 0,1);
    }
    else {
        l = vec3(1);
    }

	exColor = t_color * vec4(l, transparency);
}
