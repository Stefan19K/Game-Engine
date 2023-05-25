attribute vec3 a_posL;
attribute vec2 a_uv;
attribute vec2 a_uv2;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform sampler2D u_texture_0;

varying vec2 v_uv;
varying vec3 v_world_pos;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	vec4 world_pos = u_model * posL;

	gl_Position = u_proj * u_view * world_pos;

	v_uv = a_uv;
	v_world_pos = world_pos.xyz;
}