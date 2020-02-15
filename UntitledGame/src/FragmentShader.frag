#version 440

in vec2 out_textureCoords;
in vec3 out_normalVec;
in vec3 out_worldVertexPos;
in vec4 out_lightVertexPos;

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D shadowMap;

uniform vec3 lightColor;  // white light
uniform vec3 lightPos;  // position in world space
uniform vec3 cameraPos;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light{
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
};

uniform Light light;

float ShadowCalculation(vec4 lightVertexPos)
{
	vec3 projCoords = lightVertexPos.xyz / lightVertexPos.w;  // [-1, 1]
	projCoords = (projCoords * 0.5) + 0.5;  // [0, 1]
	float shadowBias = max(0.05 * (1.0 - dot(out_normalVec, lightPos - out_worldVertexPos)), 0.005);
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z - shadowBias;  // cure shadow acne

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);  // width and height of texture

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;

	//if(projCoords.z > 1.0)
	//{
		//shadow = 0.0f;
	//}
	
	//float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

	return shadow;
}

void main()
{	
	vec3 color = texture(tex, out_textureCoords).rgb;  // color of current pixel read from texture
	//vec3 color = vec3(0.5, 0.5, 0.5);

	// AMBIENT LIGHTING
	//float ambientStrength = 0.1f;
	//vec3 ambient = ambientStrength * lightColor;

	// new AMBIENT
	float ambientStrength = 0.2;
	vec3 ambient = light.ambientStrength * material.ambient;

	// DIFFUSE LIGHTING
	vec3 n_normalVec = normalize(out_normalVec);
	vec3 n_lightDirection = normalize(lightPos - out_worldVertexPos);
	float diffuseImpact = max(dot(n_normalVec, n_lightDirection), 0.0);  // cos of angle
	//vec3 diffuse = diffuseImpact * lightColor;

	// new DIFFUSE
	vec3 diffuse = diffuseImpact * light.diffuseStrength * material.diffuse;

	// SPECULAR LIGHTING
	float specularStrength = 0.5f;
	vec3 n_viewVector = normalize(cameraPos - out_worldVertexPos);
	vec3 reflectDir = reflect(-n_lightDirection, n_normalVec);
	float shininess = 64;  // 64
	//float spec = pow(max(dot(n_viewVector, reflectDir), 0.0), shininess);
	//vec3 specular = specularStrength * spec * lightColor;

	// new SPECULAR
	float spec = pow(max(dot(n_viewVector, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specularStrength * material.specular;
	

	// SHADOW
	float shadow = ShadowCalculation(out_lightVertexPos);
	vec3 finalColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * lightColor * color;  // with shadow
	//vec3 finalColor = (ambient + diffuse + specular);  // without shadow

	//fragColor = texture(tex, out_textureCoords) * vec4(light, 1.0f);
	fragColor = vec4(finalColor, 1.0);
}