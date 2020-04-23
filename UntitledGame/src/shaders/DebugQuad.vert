#version 440

layout (location = 0) in vec3 in_vertexPosition;
layout (location = 1) in vec2 in_textureCoords;

out vec2 out_textureCoords;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(in_vertexPosition, 1.0);
	out_textureCoords = in_textureCoords;
}