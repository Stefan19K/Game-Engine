attribute vec3 a_posL;
attribute vec3 a_norm;
attribute vec3 a_colL;
attribute vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

varying vec3 v_frag_color;
varying vec2 v_uv;
varying vec3 v_world_pos;
varying vec3 v_world_norm;
varying vec3 v_norm;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	vec4 world_pos = u_model * posL;

	vec4 normL = vec4(a_norm, 1.0);
	vec4 world_norm = u_model * normL;

	gl_Position = u_proj * u_view * world_pos;

	v_frag_color = a_colL;
	v_uv = a_uv;
	v_world_pos = world_pos.xyz;
	v_world_norm = normalize(mat3(u_model) * a_norm);
	v_norm = world_norm.xyz;
}
   