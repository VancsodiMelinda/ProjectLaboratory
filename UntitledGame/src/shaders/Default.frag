#version 440

in vec3 out_worldVertexPos;
in vec2 out_textureCoords;
in vec3 out_normalVec;

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
};
uniform DirLight dirLightArray[2];

vec3 calcDirLight();

void main()
{
	//vec3 finalColor = vec3(0.0, 0.0, 0.0);

	//for (int i = 0; i < dirLightArray.length(); i++)
	//{
		//finalColor += calcDirLight();
	//}

	fragColor = vec4(texture(material.diffuseMap, out_textureCoords).rgb, 1.0);
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

	vec3 color = ambient + diffuse + specular;

	return color;
}