#version 440
// deferred rendering/shading - 2nd (lighting) pass

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_uv;

out vec2 fs_uv;

void main()
{
	gl_Position = vec4(vs_position, 1.0);
	fs_uv = vs_uv;
}