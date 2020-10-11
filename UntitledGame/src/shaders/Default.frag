#version 440

in vec3 out_worldVertexPos;
in vec2 out_textureCoords;
in vec3 out_normalVec;
in vec3 out_tangent;
in vec3 out_bitangent;
in mat3 out_TBN;

in vec4 rawVertexPosition;
in mat4 out_modelMatrix;

out vec4 fragColor;

struct Material{ 
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	float shininess;
};
uniform Material material;
uniform int hasNormalMap;

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
const int NR_DIR_LIGHTS  = 4;
uniform DirLight dirLightArray[NR_DIR_LIGHTS];

struct PointLight{
	vec3 position;
	vec3 color;
	
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	float constant;
	float linear;
	float quadratic;

	samplerCube shadowBox;
};
const int NR_POINT_LIGHTS = 2;
uniform PointLight pointLightArray[NR_POINT_LIGHTS];

uniform samplerCube skybox;

vec3 calcDirLight(DirLight dirLight);
float calcDirShadow(DirLight dirLight);

vec3 caclPointLight(PointLight pointLight);
float calcPointShadow(PointLight pointLight);

vec3 reflection();
vec3 refraction();
vec3 depthBuffer();  // not working

void main()
{
	float intensityFactor = 1.0;
	vec3 finalColor = vec3(0.0);

	// directional lights
	//for (int i = 0; i < NR_DIR_LIGHTS; i++)
	//{
		//finalColor += ((calcDirLight(dirLightArray[i]))*intensityFactor);
	//}

	// point lights
	for (int j = 0; j < NR_POINT_LIGHTS; j++)
	{
		finalColor += caclPointLight(pointLightArray[j]);
	}


	fragColor = vec4(depthBuffer(), 1.0);
	//fragColor = vec4(finalColor, 1.0);	// frag color with lighting
	//fragColor = vec4(normalize(out_tangent), 1.0);

	//fragColor = vec4(reflection(), 1.0);
	//fragColor = vec4(refraction(), 1.0);

	//vec3 test = texture(material.normalMap, out_textureCoords).rgb;  // [0, 1]
	//fragColor = vec4(normalize(test * 2.0 - 1.0), 1.0);
	

	/*
	if (hasNormalMap == 0)
		fragColor = vec4(finalColor, 1.0);
	else
	{
		vec3 test = texture(material.normalMap, out_textureCoords).rgb;
		fragColor = vec4(normalize(test * 2.0 - 1.0), 1.0);
	}
	*/
}


vec3 calcDirLight(DirLight dirLight)
{
	// AMBIENT
	vec3 ambient = dirLight.ambientStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	
	vec3 normalVector = normalize(out_normalVec);
	if (hasNormalMap == 1)
		normalVector = normalize(out_TBN * (texture(material.normalMap, out_textureCoords).rgb * 2.0 - 1.0));
	

	// DIFFUSE
	vec3 lightDirection = normalize(-dirLight.direction);				// unit vector, points towards the light
	//vec3 normalVector = normalize(out_normalVec);						// unit vector, normal of vertex/fragment
	//vec3 normalVector = normalize(texture(material.normalMap, out_textureCoords).rgb * 2.0 - 1.0);
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

vec3 caclPointLight(PointLight pointLight)
{
	// AMBIENT
	vec3 ambient = pointLight.ambientStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	
	vec3 normalVector = normalize(out_normalVec);
	if (hasNormalMap == 1)
		normalVector = normalize(out_TBN * (texture(material.normalMap, out_textureCoords).rgb * 2.0 - 1.0));
	

	//DIFFUSE
	vec3 lightDirection = normalize(pointLight.position - out_worldVertexPos);	// unit
	//vec3 normalVector = normalize(out_normalVec);								// unit
	//vec3 normalVector = normalize(texture(material.normalMap, out_textureCoords).rgb * 2.0 - 1.0);
	float diffuseImpact = max(dot(normalVector, lightDirection), 0.0);			// cos of angle
	vec3 diffuse = diffuseImpact * pointLight.diffuseStrength * texture(material.diffuseMap, out_textureCoords).rgb;

	// SPECULAR
	vec3 viewVector = normalize(camera.position - out_worldVertexPos);
	vec3 reflectDir = reflect(-lightDirection, normalVector);
	float spec = pow(max(dot(viewVector, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * pointLight.specularStrength * texture(material.specularMap, out_textureCoords).rgb;

	float dist = length(pointLight.position - out_worldVertexPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * dist * dist);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float shadow = calcPointShadow(pointLight);
	vec3 color = (ambient + (1.0 - shadow) * (diffuse + specular)) * pointLight.color;

	return color;
}

float calcPointShadow(PointLight pointLight)
{
	vec3 lightToFrag = out_worldVertexPos - pointLight.position;
	float mapDepth = texture(pointLight.shadowBox, lightToFrag).r * camera.farPlane;
	float sceneDepth = length(lightToFrag);

	float bias = 0.05;
	float shadow = sceneDepth - bias > mapDepth ? 1.0 : 0.0;

	return shadow;
}

vec3 reflection()
{
	vec3 viewVector = normalize(out_worldVertexPos - camera.position);
	vec3 reflectionVector = reflect(viewVector, normalize(out_normalVec));

	return texture(skybox, reflectionVector).rgb;
}

vec3 refraction()
{
	float refractiveIndex_air = 1.0;
	float refractiveIndex_glass = 1.52;
	float ratio = refractiveIndex_air/refractiveIndex_glass;

	vec3 viewVector = normalize(out_worldVertexPos - camera.position);
	vec3 refractionVector = refract(viewVector, normalize(out_normalVec), ratio);
	
	return texture(skybox, refractionVector).rgb;
}

vec3 depthBuffer()
{
	// I don't use the camera's farPlane so the depth is more visible
	float depth = gl_FragCoord.z;
	float z = depth * 2.0 - 1.0;
	float near = 0.1; 
	float far  = 20.0; 
	float color = (2.0 * near * far) / (far + near - z * (far - near));

	return vec3(color) / far;

	//return vec3(gl_FragCoord.z);
}