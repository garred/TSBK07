#version 150

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vPos;

// Material related
uniform sampler2D texUnit1, texUnit2;
uniform float specularExponent;
uniform float k_d;
uniform float transparency;

// Time
uniform float time;

// lights
#define N_LIGHTS 8
uniform bool isActive[N_LIGHTS];
uniform vec3 lightSourcesDirPosArr[N_LIGHTS];
uniform vec3 lightSourcesColorArr[N_LIGHTS];
uniform float lightRange[N_LIGHTS];
uniform float ambientFactor[N_LIGHTS];
uniform bool isDirectional[N_LIGHTS];
uniform vec3 cameraWorldPos;


out vec4 exColor;


void main(void)
{
	// Texture color
	vec4 t_color = texture(texUnit1, vTexCoord) + texture(texUnit2, vTexCoord);


	vec3 i_amb = vec3(0.0);
	vec3 i_diff = vec3(0.0);
	vec3 i_spec = vec3(0.0);

	for (int i=0; i<N_LIGHTS; i++) {
		if (isActive[i]) {
			vec3 l_dir;
			float l_d;
			if (isDirectional[i]) {
				l_dir = lightSourcesDirPosArr[i];
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

	vec3 l = i_amb + i_diff + i_spec;

	exColor = t_color * vec4(l, transparency);
}
