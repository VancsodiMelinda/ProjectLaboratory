#version 440

in vec2 out_textureCoords;
in vec3 out_normalVec;
in vec3 out_worldVertexPos;
in vec4 out_lightVertexPos;

out vec4 fragColor;

//uniform sampler2D tex;
uniform sampler2D shadowMap;

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
	
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
};
uniform DirLight dirLight;

struct PointLight{
	vec3 position;

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
float ShadowCalculation();
vec3 CalcDirLight();

void main()
{	
	//vec3 color = texture(tex, out_textureCoords).rgb;  // color of current pixel read from texture

	//vec3 lightDirection = normalize(-light.direction);

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
	
	

	// POINT LIGHT
	float distance = length(pointLight.position - out_worldVertexPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// SHADOW
	float shadow = ShadowCalculation();
	vec3 finalColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * lightColor;  // with shadow
	//vec3 finalColor = (ambient + diffuse + specular);  // without shadow
	//vec3 finalColor = CalcDirLight();

	fragColor = vec4(finalColor, 1.0);
}




//float ShadowCalculation(vec4 lightVertexPos)
float ShadowCalculation()
{
	//vec3 projCoords = lightVertexPos.xyz / lightVertexPos.w;  // [-1, 1]
	vec3 projCoords = out_lightVertexPos.xyz / out_lightVertexPos.w;  // [-1, 1]
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


vec3 CalcDirLight()
{
	// AMBIENT
	vec3 ambient = dirLight.ambientStrength * texture(material.diffuseMap, out_textureCoords).rgb;	// new

	// DIFFUSE
	vec3 lightDirection = normalize(-dirLight.direction);	// unit
	vec3 normalVector = normalize(out_normalVec);					// unit
	float diffuseImpact = max(dot(normalVector, lightDirection), 0.0);  // cos of angle
	vec3 diffuse = diffuseImpact * dirLight.diffuseStrength * texture(material.diffuseMap, out_textureCoords).rgb;  // new

	// SPECULAR
	vec3 viewVector = normalize(cameraPos - out_worldVertexPos);
	vec3 reflectDir = reflect(-lightDirection, normalVector);
	float spec = pow(max(dot(viewVector, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * dirLight.specularStrength * texture(material.specularMap, out_textureCoords).rgb;	// new
	
	return (ambient + diffuse + specular);
}

