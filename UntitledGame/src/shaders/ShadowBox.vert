#version 440

layout (location = 0) in vec3 vertexPos;

uniform mat4 modelMatrix;

void main()
{
	gl_Position = modelMatrix * vec4(vertexPos, 1.0);
}
