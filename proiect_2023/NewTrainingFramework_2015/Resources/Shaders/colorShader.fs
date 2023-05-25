precision mediump float;

varying vec3 v_frag_color;
varying vec2 v_uv;

void main()
{
	gl_FragColor = vec4(v_frag_color, 1.0);
}
