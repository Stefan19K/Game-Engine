precision mediump float;

uniform sampler2D u_texture;

varying vec3 v_frag_color;
varying vec2 v_uv;

void main()
{
	gl_FragColor = texture2D(u_texture, v_uv);
	// gl_FragColor = vec4(v_uv.x, v_uv.y, 0.0, 1.0);
}
