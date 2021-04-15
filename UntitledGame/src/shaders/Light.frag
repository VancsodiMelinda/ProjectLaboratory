#version 440

out vec4 fragColor;

uniform vec3 lightColor;  // white light
uniform int ID;

layout (location = 0) out vec3 standardColor;
layout (location = 1) out vec4 selectionColor;

vec4 calcSelectionColor();

void main()
{
	//fragColor = vec4(1.0f);  // white light
	fragColor = vec4(lightColor, 1.0f);

	standardColor = lightColor;
	selectionColor = calcSelectionColor();
}

vec4 calcSelectionColor()
{
	int R = (ID & 0x000000FF) >>  0;
	int G = (ID & 0x0000FF00) >>  8;
	int B = (ID & 0x00FF0000) >> 16;

	return vec4(R/255.0, G/255.0, B/255.0, 1.0);
	//return vec4(1.0, 0.0, 0.0, 1.0);
}