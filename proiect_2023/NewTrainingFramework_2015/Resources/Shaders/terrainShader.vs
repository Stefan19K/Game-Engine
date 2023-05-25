attribute vec3 a_posL;
attribute vec3 a_norm;
attribute vec2 a_uv;
attribute vec2 a_uv2;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform sampler2D u_texture_0;
uniform vec3 u_height;
uniform vec2 u_uv_offset;

varying vec3 v_frag_color;
varying vec2 v_uv;
varying vec2 v_uv2;
varying vec3 v_world_pos;
varying vec3 v_world_norm;

void main()
{
	vec2 uv2;
	uv2 = a_uv2 + u_uv_offset;
	vec4 c_blend = texture2D(u_texture_0, uv2);

	vec4 posL = vec4(a_posL, 1.0);
	posL.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;
	vec4 world_pos = u_model * posL;

	gl_Position = u_proj * u_view * world_pos;

	v_uv = a_uv;
	v_uv2 = uv2;
	v_world_pos = world_pos.xyz;
	v_world_norm = normalize(mat3(u_model) * a_norm);
}