#version 440

out vec4 fragColor;

void main()
{
	vec3 outlineColor = vec3(0.4, 0.7, 1.0);
	fragColor = vec4(outlineColor, 1.0);
}