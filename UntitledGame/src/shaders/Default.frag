#version 440

in vec3 out_worldVertexPos;
in vec2 out_textureCoords;
in vec3 out_normalVec;

in vec4 rawVertexPosition;
in mat4 out_modelMatrix;

out vec4 fragColor;

struct Material{ 
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};
uniform Material material;

struct Camera{
	vec3 position;
	float farPlane;
};
uniform Camera camera;

struct DirLight{
	vec3 direction;
	vec3 color;
	
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	sampler2D shadowMap;
	mat4 lightSpaceMatrix;
};
uniform DirLight dirLightArray[4];

vec3 calcDirLight(DirLight dirLight);
float calcDirShadow(DirLight dirLight);

void main()
{
	int numberOfLights = 2;
	vec3 finalColor = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < dirLightArray.length(); i++)
	{
		finalColor += (calcDirLight(dirLightArray[i]))/numberOfLights;
	}

	//finalColor = texture(dirLightArray[1].shadowMap, out_textureCoords).rgb;

	fragColor = vec4(finalColor, 1.0);
}


vec3 calcDirLight(DirLight dirLight)
{
	// AMBIENT
	vec3 ambient = dirLight.ambientStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	// DIFFUSE
	vec3 lightDirection = normalize(-dirLight.direction);				// unit vector, points towards the light
	vec3 normalVector = normalize(out_normalVec);						// unit vector, normal of vertex/fragment
	float diffuseImpact = max(dot(normalVector, lightDirection), 0.0);  // cos of angle
	vec3 diffuse = diffuseImpact * dirLight.diffuseStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	// SPECULAR
	vec3 viewVector = normalize(camera.position - out_worldVertexPos);	// unit vector, points towards the camera
	vec3 reflectDir = reflect(-lightDirection, normalVector);			// unit vector, reflected normal of vertex/fragment
	float spec = pow(max(dot(viewVector, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * dirLight.specularStrength * texture(material.specularMap, out_textureCoords).rgb;


	float shadow = calcDirShadow(dirLight);
	vec3 color = (ambient + (1.0 - shadow) * (diffuse + specular)) * dirLight.color;
	//vec3 color = (ambient + diffuse + specular) * dirLight.color;

	return color;
}

float calcDirShadow(DirLight dirLight)
{
	vec4 out_lightVertexPos =  dirLight.lightSpaceMatrix * out_modelMatrix * rawVertexPosition;
	vec3 projCoords = out_lightVertexPos.xyz / out_lightVertexPos.w;	// [-1, 1]
	projCoords = (projCoords * 0.5) + 0.5;								// [0, 1]
	//float shadowBias = max(0.05 * (1.0 - dot(out_normalVec, lightPosition - out_worldVertexPos)), 0.005);  // annyira nem tunik jonak
	float shadowBias = max(0.05 * (1.0 - dot(out_normalVec, (-dirLight.direction) - out_worldVertexPos)), 0.005);
	float closestDepth = texture(dirLight.shadowMap, projCoords.xy).r;	// depth in scene
	float currentDepth = projCoords.z - shadowBias;						// cure shadow acne
	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(dirLight.shadowMap, 0);  // width and height of texture

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(dirLight.shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;	// depth in shadow map
			shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;

	//shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

	return shadow;
}