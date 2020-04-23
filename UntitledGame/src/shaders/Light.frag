#version 440

out vec4 fragColor;

uniform vec3 lightColor;  // white light

void main()
{
	//fragColor = vec4(1.0f);  // white light
	fragColor = vec4(lightColor, 1.0f);
}