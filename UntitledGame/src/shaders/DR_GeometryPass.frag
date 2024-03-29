#version 440
// deferred rendering/shading - 1st (geometry) pass

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpecular;

in vec3 fs_worldPosition;
in vec2 fs_uv;
in vec3 fs_normal;
in mat3 fs_TBN;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
//uniform sampler2D specularMap;

void main()
{
	/*
	gPosition = fs_worldPosition;
	gNormal = normalize(fs_normal);
	gAlbedoSpecular.rgb = texture(diffuseMap, fs_uv).rgb;
	gAlbedoSpecular.a = 0.0f;
	*/

	//
	gPosition = vec4(fs_worldPosition, 1.0);
	gNormal = vec4(normalize(fs_TBN * (texture(normalMap, fs_uv).rgb * 2.0 - 1.0)), 1.0);
	gAlbedoSpecular = vec4(texture(diffuseMap, fs_uv).rgb, 1.0);
}