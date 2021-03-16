#version 440
// deferred rendering/shading - 1st (geometry) pass

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
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

	// normal from normal map - enhance normal map
	//gNormal = normalize(fs_TBN * (texture(normalMap, fs_uv).rgb * 2.0 - 1.0));

	// specular from specular map
	//gAlbedoSpecular.a = texture(specularMap, fs_uv).r;

	gPosition = fs_worldPosition;
	gNormal = normalize(fs_normal);
	gAlbedoSpecular.rgb = vec3(1.0);
	gAlbedoSpecular.a = 0.0f;
}