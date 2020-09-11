#version 440

layout (location = 0) in vec3 in_vertexPosition;
layout (location = 1) in vec2 in_textureCoords;
layout (location = 2) in vec3 in_normalVec;

out vec3 out_worldVertexPos;
out vec2 out_textureCoords;
out vec3 out_normalVec;

uniform mat4 MVP;
uniform mat4 modelMatrix;

void main()
{
	gl_Position = MVP * vec4(in_vertexPosition, 1.0);

	out_worldVertexPos = vec3(modelMatrix * vec4(in_vertexPosition, 1.0));
	out_textureCoords = in_textureCoords;
	mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
	out_normalVec = normalMatrix * in_normalVec;
}