#version 440

in vec4 fragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	float lightDist = length(fragPos.xyz - lightPos);
	lightDist = lightDist / farPlane;	// [0, 1]

	gl_FragDepth = lightDist;
}
