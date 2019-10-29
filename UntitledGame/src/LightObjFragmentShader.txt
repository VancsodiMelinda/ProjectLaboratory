#version 440

layout (location = 0) in vec3 in_vertexPosition;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(in_vertexPosition, 1.0);
}