#version 440

in vec2 out_textureCoords;

out vec4 fragColor;

uniform sampler2D shadowMap;

void main()
{
	float depthValue = texture(shadowMap, out_textureCoords).r;
	fragColor = vec4(vec3(depthValue), 1.0);
	//vec3 col = texture(shadowMap, out_textureCoords).rgb;
	//fragColor = vec4(col, 1.0);
}