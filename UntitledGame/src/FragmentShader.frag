#version 440

in vec2 out_textureCoords;
in vec3 out_normalVec;
in vec3 out_worldVertexPos;
in vec4 out_lightVertexPos;

out vec4 fragColor;

//uniform sampler2D tex;
uniform sampler2D shadowMap;
uniform sampler2D projectiveMap;

uniform vec3 lightColor;  // white light
uniform vec3 lightPos;  // position in world space
uniform vec3 cameraPos;

struct Material{ 
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};
uniform Material material;

struct Light{
	vec3 color;
	vec3 position;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
};
uniform Light light;

struct DirLight{
	vec3 direction;
	vec3 color;
	
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
};
uniform DirLight dirLight;

struct PointLight{
	vec3 position;
	vec3 color;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	float constant;
	float linear;
	float quadratic;
};
uniform PointLight pointLight;


// function definitons
//float ShadowCalculation(vec4 lightVertexPos);
float ShadowCalculation(vec3 lightPosition);
vec3 CalcDirLight();
vec3 CalcPointLight();
vec3 ProjectiveTextureMapping(vec3 currentColor, vec3 lightPosition);

void main()
{	
	//vec3 color = texture(tex, out_textureCoords).rgb;  // color of current pixel read from texture

	//vec3 lightDirection = normalize(-light.direction);
	/*
	// AMBIENT LIGHTING
	//vec3 ambient = light.ambientStrength * material.ambient;
	vec3 ambient = light.ambientStrength * texture(material.diffuseMap, out_textureCoords).rgb;	// new

	// DIFFUSE LIGHTING
	vec3 n_normalVec = normalize(out_normalVec);
	vec3 n_lightDirection = normalize(lightPos - out_worldVertexPos);
	float diffuseImpact = max(dot(n_normalVec, n_lightDirection), 0.0);  // cos of angle
	//vec3 diffuse = diffuseImpact * light.diffuseStrength * material.diffuse;
	vec3 diffuse = diffuseImpact * light.diffuseStrength * texture(material.diffuseMap, out_textureCoords).rgb;  // new

	// SPECULAR LIGHTING
	float specularStrength = 0.5f;
	vec3 n_viewVector = normalize(cameraPos - out_worldVertexPos);
	vec3 reflectDir = reflect(-n_lightDirection, n_normalVec);
	float spec = pow(max(dot(n_viewVector, reflectDir), 0.0), material.shininess);
	//vec3 specular = spec * light.specularStrength * material.specular;
	vec3 specular = spec * light.specularStrength * texture(material.specularMap, out_textureCoords).rgb;	// new
	*/
	

	// POINT LIGHT
	//float distance = length(pointLight.position - out_worldVertexPos);
	//float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
	//ambient *= attenuation;
	//diffuse *= attenuation;
	//specular *= attenuation;

	// SHADOW
	//float shadow = ShadowCalculation();
	//vec3 finalColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * lightColor;  // with shadow
	//vec3 finalColor = (ambient + diffuse + specular);  // without shadow

	vec3 finalColor = CalcDirLight();		// DIRECTIONAL LIGHT
	//vec3 finalColor = CalcPointLight();	// POINT LIGHT
	//vec3 otherColor = ProjectiveTextureMapping();
	//vec3 otherColor = ProjectiveTextureMapping(finalColor);

	fragColor = vec4(finalColor, 1.0);
	//fragColor = vec4(otherColor, 1.0);
}

vec3 ProjectiveTextureMapping(vec3 currentColor, vec3 lightVector)
{
	vec3 projCoords = out_lightVertexPos.xyz / out_lightVertexPos.w;	// [-1, 1]
	projCoords = (projCoords * 0.5) + 0.5;								// [0, 1]

	vec3 pixelColor = vec3(0.0, 0.0, 0.0);
	float multiplier = 0.0;

	//float shadowBias = max(0.05 * (1.0 - dot(out_normalVec, lightVector)), 0.005);
	float bias = max(0.05 * (1.0 - dot(normalize(out_normalVec), lightVector)), 0.005);
	float closestDepth = texture(shadowMap, projCoords.xy).r;	// depth in shadow map
	float currentDepth = projCoords.z;			// depth in scene

	bool condition1 = (0.0 <= projCoords.x) && (projCoords.x <= 1.0) && (0.0 <= projCoords.y) && (projCoords.y <= 1.0);	// if vertex is in the frustum
	bool condition2 = dot(normalize(out_normalVec), lightVector) >= 0.0;	// if vertex is "facing" the lightsource
	bool condition3 = (currentDepth - bias) < closestDepth;	// not in shadow

	float diffuseImpact = max(dot(normalize(out_normalVec), lightVector), 0.0);

	//if (condition1 && condition2 && condition3)
	if (condition1 && condition3)
	{
		//pixelColor = diffuseImpact * vec3(1.0, 0.0, 0.0);
		pixelColor = diffuseImpact * texture(projectiveMap, projCoords.xy).rgb;	// without condition2
		//pixelColor = vec3(1.0, 0.0, 0.0);
		//pixelColor = texture(projectiveMap, projCoords.xy).rgb;	// with condition2
		multiplier = 0.5;
	}

	//vec3 pixelColor = texture(material.diffuseMap, projCoords.xy).rgb;

	//float shadow = currentDepth > pcfDepth ? 1.0 : 0.0;  // 1 if its in shadow, 0 if its visible

	return ((multiplier * pixelColor) + ((1.0 - multiplier) * currentColor));
}

float ShadowCalculation(vec3 lightPosition)
{
	vec3 projCoords = out_lightVertexPos.xyz / out_lightVertexPos.w;	// [-1, 1]
	projCoords = (projCoords * 0.5) + 0.5;								// [0, 1]
	float shadowBias = max(0.05 * (1.0 - dot(out_normalVec, lightPosition - out_worldVertexPos)), 0.005);  // annyira nem tunik jonak
	float closestDepth = texture(shadowMap, projCoords.xy).r;			// depth in scene
	float currentDepth = projCoords.z - shadowBias;						// cure shadow acne

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);  // width and height of texture

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;	// depth in shadow map
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


vec3 CalcDirLight()
{
	// AMBIENT
	vec3 ambient = dirLight.ambientStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	// DIFFUSE
	vec3 lightDirection = normalize(-dirLight.direction);				// unit vector, points towards the light
	vec3 normalVector = normalize(out_normalVec);						// unit vector, normal of vertex/fragment
	float diffuseImpact = max(dot(normalVector, lightDirection), 0.0);  // cos of angle
	vec3 diffuse = diffuseImpact * dirLight.diffuseStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	// SPECULAR
	vec3 viewVector = normalize(cameraPos - out_worldVertexPos);		// unit vector, points towards the camera
	vec3 reflectDir = reflect(-lightDirection, normalVector);			// unit vector, reflected normal of vertex/fragment
	float spec = pow(max(dot(viewVector, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * dirLight.specularStrength * texture(material.specularMap, out_textureCoords).rgb;
	
	float shadow = ShadowCalculation(lightDirection);					// 1 = in shadow, 0 = visible from light

	vec3 currentColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * dirLight.color;

	vec3 otherColor = ProjectiveTextureMapping(currentColor, lightDirection);

	//return (ambient + diffuse + specular);
	//return (ambient + (1.0 - shadow) * (diffuse + specular)) * dirLight.color;
	return otherColor;
	//return currentColor;
}


vec3 CalcPointLight()
{
	// AMBIENT
	vec3 ambient = pointLight.ambientStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	//DIFFUSE
	vec3 lightDirection = normalize(pointLight.position - out_worldVertexPos);	// unit
	vec3 normalVector = normalize(out_normalVec);					// unit
	float diffuseImpact = max(dot(normalVector, lightDirection), 0.0);  // cos of angle
	vec3 diffuse = diffuseImpact * pointLight.diffuseStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	// SPECULAR
	vec3 viewVector = normalize(cameraPos - out_worldVertexPos);
	vec3 reflectDir = reflect(-lightDirection, normalVector);
	float spec = pow(max(dot(viewVector, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * pointLight.specularStrength * texture(material.specularMap, out_textureCoords).rgb;

	float dist = length(pointLight.position - out_worldVertexPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * dist * dist);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float shadow = ShadowCalculation(pointLight.position);

	//return (ambient + diffuse + specular);
	return (ambient + (1.0 - shadow) * (diffuse + specular)) * pointLight.color;
}
