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


void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//fragColor = vec4(texture(material.diffuseMap, out_textureCoords).rgb, 1.0);
}
