#version 440

out vec4 fragColor;

uniform vec3 lightColor;  // white light

layout (location = 0) out vec3 standardColor;
layout (location = 1) out vec3 selectionColor;

void main()
{
	//fragColor = vec4(1.0f);  // white light
	fragColor = vec4(lightColor, 1.0f);

	standardColor = lightColor;
	selectionColor = vec3(1.0, 0.0, 0.0);
}