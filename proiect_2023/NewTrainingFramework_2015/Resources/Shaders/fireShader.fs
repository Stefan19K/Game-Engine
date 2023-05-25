precision mediump float;

// Texture related variables
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
varying vec2 v_uv;

// Fog related variables
uniform float u_min_field;
uniform float u_max_field;
uniform vec3 u_fog_color;
uniform vec3 u_cam_pos;
varying vec3 v_world_pos;

// Fire related variables
uniform float u_time;
uniform float u_dispMax;

void main()
{
	float d = distance(u_cam_pos, v_world_pos);
	float alpha = clamp(d, u_min_field, u_max_field);
	alpha = (alpha - u_min_field) / (u_max_field - u_min_field);

	vec2 disp = texture2D(u_texture_0, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offset = (disp * 2.0 - 1.0) * u_dispMax;
	vec2 v_uv_displaced = v_uv + offset;
	vec4 c_fire = texture2D(u_texture_1, v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture_2, v_uv);
	c_fire.a = c_fire.a * c_alpha.r;
	vec3 col = alpha * u_fog_color + (1.0 - alpha) * c_fire.xyz;

	gl_FragColor = vec4(col, c_fire.a);
	// gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
