#version 440

in VS_OUT {
	vec2 uvs;
} fs_in;

uniform sampler2D screenTexture;
uniform sampler2D selectionTexture;

out vec4 fragColor;

vec4 showSelectionTexture();

vec4 normal();
vec4 inversion();
vec4 grayscale();
vec4 nightVision();
vec4 calcKernel(float kernel[9]);
vec4 sharpen();
vec4 blur();
vec4 edge();
vec4 emboss();
vec4 pixelate();

void main()
{
	fragColor = showSelectionTexture();
	//fragColor = inversion();
}

vec4 showSelectionTexture()
{
	return texture(selectionTexture, fs_in.uvs);
}

vec4 normal()
{
	return texture(screenTexture, fs_in.uvs);
}

vec4 inversion()
{
	return vec4(vec3(1.0 - texture(screenTexture, fs_in.uvs)), 1.0);
}

vec4 grayscale()
{
	vec4 color = texture(screenTexture, fs_in.uvs);
	float average = (color.r + color.g + color.b) / 3.0;
	//float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	return vec4(average, average, average, 1.0);
}

vec4 nightVision()
{
	vec4 color = texture(screenTexture, fs_in.uvs);
	float average = (color.r + color.g + color.b) / 3.0;
	return vec4(0.0, average, 0.0, 1.0);
}

vec4 calcKernel(float kernel[9])
{
	const float offset = 1.0 / 300.0;

	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

	/*
	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);
	*/

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, fs_in.uvs + offsets[i]));
	}

	vec3 color = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		color += sampleTex[i] * kernel[i];
	}

	return vec4(color, 1.0);
}

vec4 sharpen()
{
	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	return calcKernel(kernel);
}

vec4 blur()
{
	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);

	return calcKernel(kernel);
}

vec4 edge()
{
	float kernel[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
	);

	return calcKernel(kernel);
}

vec4 emboss()
{
	float kernel[9] = float[](
		-2, -1, 0,
		-1, 1, 1,
		 0, 1, 2
	);

	return calcKernel(kernel);
}

vec4 pixelate()
{
	// source: http://coding-experiments.blogspot.com/2010/06/pixelation.html
	float param = 10.0;  // at 100.0 you get back the original resolution (1920x1080) I think
	float uPixels = 192.0 * param;
	float vPixels = 108.0 * param;
    float dx = 10.0 * (1.0 / uPixels);
    float dy = 10.0 * (1.0 / vPixels);
    vec2 Coord = vec2(dx * floor(fs_in.uvs.x / dx), dy * floor(fs_in.uvs.y / dy));
    vec3 finalColor = texture(screenTexture, Coord).rgb;

	return vec4(finalColor, 1.0);
	//return vec4(fs_in.uvs, 0.0, 1.0);
}