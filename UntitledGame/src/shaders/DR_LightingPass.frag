#version 440
// deferred rendering/shading - 2nd (lighting) pass

in vec2 fs_uv;

out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpecular;

struct Light{
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};
const int NR_LIGHTS = 33;
uniform Light lights[NR_LIGHTS];

uniform vec3 cameraPos;

void main()
{
	vec3 position = texture(gPosition, fs_uv).rgb;
	vec3 normal = texture(gNormal, fs_uv).rgb;
	vec3 albedo = texture(gAlbedoSpecular, fs_uv).rgb;
	float specular = texture(gAlbedoSpecular, fs_uv).a;
	
	vec3 finalColor = vec3(0.0);

	for (int i = 0; i < NR_LIGHTS; i++)
	{
		// ambient
		vec3 ambient = 0.0 * albedo;

		// diffuse
		vec3 lightDirection = normalize(lights[i].position - position);
		float diffuseImpact = max(dot(normal, lightDirection), 0.0);
		vec3 diffuse = diffuseImpact * albedo;

		// specular
		vec3 viewVector = normalize(cameraPos - position);
		vec3 reflectDir = reflect(-lightDirection, normal);
		float spec = pow(max(dot(viewVector, reflectDir), 0.0), 16.0);
		vec3 specular = spec * specular * albedo;

		// attenuation
		float dist = length(lights[i].position - position);
		float attenuation = 1.0 / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * dist * dist);

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		finalColor += ((ambient + diffuse + specular) * lights[i].color);
	}

	fragColor = vec4(finalColor, 1.0);
	

	/*
	vec3 lighting = albedo * 0.1;
	vec3 viewDir = normalize(cameraPos - position);
	for (int i = 0; i < NR_LIGHTS; i++)
	{
		vec3 lightDir = normalize(lights[i].position - position);
		vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * lights[i].color;
		lighting += diffuse;
	}

	fragColor = vec4(lighting, 1.0);
	*/
}