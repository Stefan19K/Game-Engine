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

uniform samplerCube u_texture_0;
uniform vec3 u_cam_pos;

// Light related variables
uniform vec3 u_amb_color;
uniform float u_ka;
uniform int u_lightCount;
uniform Light u_lights[10];

varying vec3 v_coord;
varying vec3 v_world_pos;
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
    float factor = 1.0 / (10.0 + 0.1 * d + 0.11 * pow(d, 2.0));

    color = object_color * (diffuse_light * light.diffColor + specular_light * light.specColor);

    return color;
}

void main()
{
	vec4 texture_color = textureCube(u_texture_0, v_coord);

	gl_FragColor = texture_color;
	// gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
