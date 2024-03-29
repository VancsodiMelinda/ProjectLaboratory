#version 440

layout (location = 0) in vec3 in_vertexPosition;
layout (location = 1) in vec2 in_textureCoords;
layout (location = 2) in vec3 in_normalVec;

out vec2 out_textureCoords;
out vec3 out_normalVec;
out vec3 out_worldVertexPos;
out vec4 out_lightVertexPos;

//uniform mat4 scalingMatrix;
//uniform mat4 rotationMatrix;
//uniform mat4 translationMatrix;
//uniform mat4 projectionMatrix;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

void main()
{
	//gl_Position = projectionMatrix * rotationMatrix * scalingMatrix * vec4(in_vertexPosition, 1.0);
	//gl_Position = projectionMatrix * translationMatrix * rotationMatrix * scalingMatrix * vec4(in_vertexPosition, 1.0);

	gl_Position = MVP * vec4(in_vertexPosition, 1.0);
	out_textureCoords = in_textureCoords;
	//out_normalVec = in_normalVec;
	mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
	out_normalVec = normalMatrix * in_normalVec;
	out_worldVertexPos = vec3(modelMatrix * vec4(in_vertexPosition, 1.0));

	//out_lightVertexPos = lightSpaceMatrix * vec4(out_worldVertexPos, 1.0);
	out_lightVertexPos = lightSpaceMatrix * vec4(in_vertexPosition, 1.0);
}