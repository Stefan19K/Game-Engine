attribute vec3 a_posL;
attribute vec3 a_norm;
attribute vec3 a_colL;
attribute vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

varying vec3 v_coord;
varying vec3 v_world_pos;
varying vec3 v_world_norm;

void main()
{
	vec4 normL = vec4(a_norm, 1.0);
	vec4 world_norm = u_model * normL;

	vec4 posL = vec4(a_posL, 1.0);
	vec4 world_pos = u_model * posL;

	gl_Position = u_proj * u_view * world_pos;
	v_coord = a_posL;
	v_world_pos = world_pos.xyz;
	v_world_norm = world_norm.xyz;
}
   