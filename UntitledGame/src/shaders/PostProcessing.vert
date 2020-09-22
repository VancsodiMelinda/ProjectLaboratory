#version 440

layout (location = 0) in vec3 vertices;
layout (location = 1) in vec2 uvs;

out VS_OUT {
	vec2 uvs;
} vs_out;

void main()
{
	gl_Position = vec4(vertices, 1.0);
	vs_out.uvs = uvs;
}