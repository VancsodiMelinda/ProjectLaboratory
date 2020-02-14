#version 440

layout (location = 0) in vec2 in_vertexPosition;
layout (location = 1) in vec2 in_textureCoords;

out vec2 out_textureCoords;

void main()
{
	gl_Position = vec4(in_vertexPosition, 0.0, 1.0);
	out_textureCoords = in_textureCoords;
}