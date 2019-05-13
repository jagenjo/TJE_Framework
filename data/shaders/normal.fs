
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

void main()
{
	gl_FragColor = vec4(abs(normalize(v_normal)),1.0);
}
