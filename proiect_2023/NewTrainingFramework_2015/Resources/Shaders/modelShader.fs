precision mediump float;

struct Light {
	int type;
	vec3 pos;
	vec3 direction;
	vec3 diffColor;
	vec3 specColor;
	float kd;
	float ks;
	float shininess;
	float spotAngle;
};

uniform sampler2D u_texture_0;
uniform samplerCube u_texture_1;

// Light related variables
uniform vec3 u_amb_color;
uniform float u_ka;
uniform int u_lightCount;
uniform Light u_lights[10];

// Fog related variables
uniform float u_min_field;
uniform float u_max_field;
uniform vec3 u_fog_color;
uniform vec3 u_cam_pos;
varying vec3 v_world_pos;

varying vec3 v_frag_color;
varying vec2 v_uv;
varying vec3 v_world_norm;

vec3 computeLight(Light light, vec3 object_color, vec3 world_pos, vec3 world_normal)
{
    vec3 L = normalize( light.pos - v_world_pos );

    vec3 N = normalize(v_world_norm);

    vec3 V = normalize( u_cam_pos - v_world_pos );

    vec3 R = reflect(-L, N);

    float diffuse_light =  light.kd * max(dot(N, L), 0.0);

    float specular_light = 0.0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0.0)
    {
        specular_light = light.ks * pow(max(dot(V, R), 0.0), 30.0);
    }

    vec3 color;
    float d = distance(light.pos, v_world_pos);
    float factor = 1.0 / (0.1 + 0.1 * d + 0.0 * pow(d, 2.0));

    color = object_color * (diffuse_light * light.diffColor + specular_light * light.specColor) * factor;

    return color;
}

void main()
{
	float d = distance(u_cam_pos, v_world_pos);
	float alpha = clamp(d, u_min_field, u_max_field);
	alpha = (alpha - u_min_field) / (u_max_field - u_min_field);
	vec4 texture_color = texture2D(u_texture_0, v_uv);

	vec3 vecCam = u_cam_pos - v_world_pos;
	vec3 dirReflect = reflect(normalize(vecCam), normalize(v_world_norm));
	vec4 c_reflected = textureCube(u_texture_1, dirReflect);

	texture_color = c_reflected * 0.2 + texture_color * 0.8;
	vec3 c_amb = texture_color.xyz * u_amb_color;
	vec3 col1 = c_amb * u_ka;

	vec3 col2 = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < 10; i++) {
		if (i >= u_lightCount)
			break;

		col2 += computeLight(u_lights[i], texture_color.xyz, v_world_pos, v_world_norm);
	}
	col2 = (1.0 - u_ka) * col2;

	vec3 col = col1 + col2;
	col = alpha * u_fog_color + (1.0 - alpha) * col;

	gl_FragColor = vec4(col, 1.0);
}
