attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec4 a_color;

attribute vec4 a_bones;
attribute vec4 a_weights;

uniform vec3 u_camera_pos;

uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform mat4 u_bones[128];

//this will store the color for the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

void main()
{	
	//apply skinning
	vec4 v = vec4(a_vertex,1.0);
	v_position =	(u_bones[int(a_bones.x)] * a_weights.x * v + 
			u_bones[int(a_bones.y)] * a_weights.y * v + 
			u_bones[int(a_bones.z)] * a_weights.z * v + 
			u_bones[int(a_bones.w)] * a_weights.w * v).xyz;

	vec4 N = vec4(a_normal,0.0);
	v_normal =	(u_bones[int(a_bones.x)] * a_weights.x * N + 
			u_bones[int(a_bones.y)] * a_weights.y * N + 
			u_bones[int(a_bones.z)] * a_weights.z * N + 
			u_bones[int(a_bones.w)] * a_weights.w * N).xyz;
	v_normal = normalize(v_normal);

	//calcule the normal in world space
	v_normal = (u_model * vec4( v_normal, 0.0) ).xyz;
	
	//calcule the vertex in world space
	v_world_position = (u_model * vec4( v_position, 1.0) ).xyz;
	
	//store the color in the varying var to use it from the pixel shader
	v_color = a_weights;

	//store the texture coordinates
	v_uv = a_uv;

	//calcule the position of the vertex using the matrices
	gl_Position = u_viewprojection * vec4( v_world_position, 1.0 );
}