#version 440

layout (location = 0) in vec3 vertexPosition;

out vec3 textureCoords;

uniform mat4 MVP;

void main()
{
	textureCoords = vertexPosition;
	vec4 position = MVP * vec4(vertexPosition, 1.0);
	gl_Position = position.xyww;
}
