#version 440

layout (location = 0) in vec3 in_vertexPosition;
layout (location = 1) in vec2 in_textureCoords;
layout (location = 2) in vec3 in_normalVec;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_bitangent;

out vec3 out_worldVertexPos;
out vec2 out_textureCoords;
out vec3 out_normalVec;
out vec3 out_tangent;
out vec3 out_bitangent;
out mat3 out_TBN;

out vec4 rawVertexPosition;
out mat4 out_modelMatrix;

uniform mat4 MVP;
uniform mat4 modelMatrix;

void main()
{
	gl_Position = MVP * vec4(in_vertexPosition, 1.0);

	out_worldVertexPos = vec3(modelMatrix * vec4(in_vertexPosition, 1.0));
	out_textureCoords = in_textureCoords;
	mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
	out_normalVec = normalMatrix * in_normalVec;
	rawVertexPosition = vec4(in_vertexPosition, 1.0);
	out_modelMatrix = modelMatrix;

	vec3 T = normalize(vec3(modelMatrix * vec4(in_tangent, 0.0)));
	vec3 B = normalize(vec3(modelMatrix * vec4(in_bitangent, 0.0)));
	vec3 N = normalize(vec3(modelMatrix * vec4(in_normalVec, 0.0)));

	mat3 TBN = mat3(T, B, N);
	out_TBN = TBN;

	out_tangent = in_tangent;
	out_bitangent = in_bitangent;
}