attribute vec3 a_posL;
attribute vec3 a_colL;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

varying vec3 v_frag_color;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_proj * u_view * u_model * posL;
	v_frag_color = a_colL;
}
   