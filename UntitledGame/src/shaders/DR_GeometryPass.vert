#version 440
// deferred rendering/shading - 1st (geometry) pass

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_uv;
layout (location = 2) in vec3 vs_normal;
layout (location = 3) in vec3 vs_tangent;
layout (location = 4) in vec3 vs_bitangent;

uniform mat4 MVP;
uniform mat4 M;

out vec3 fs_worldPosition;
out vec2 fs_uv;
out vec3 fs_normal;
out mat3 fs_TBN;

void main()
{
	gl_Position = MVP * vec4(vs_position, 1.0);

	// position
	fs_worldPosition = (M * vec4(vs_position, 1.0)).xyz;

	// uv
	fs_uv = vs_uv;

	// normal
	mat3 normalMatrix = mat3(transpose(inverse(M)));
	fs_normal = normalMatrix * vs_normal;

	// normal from normal map
	vec3 T = normalize(vec3(M * vec4(vs_tangent, 0.0)));
	vec3 B = normalize(vec3(M * vec4(vs_bitangent, 0.0)));
	vec3 N = normalize(vec3(M * vec4(vs_normal, 0.0)));
	mat3 TBN = mat3(T, B, N);
	fs_TBN = TBN;
}