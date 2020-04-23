#version 440

in vec3 textureCoords;

out vec4 fragColor;

uniform samplerCube skybox;

void main()
{
	fragColor = texture(skybox, textureCoords);
}