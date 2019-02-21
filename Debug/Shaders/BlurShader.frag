#version 460 core

layout(location = 0) out vec4 out_color;

layout (location = 6) uniform sampler2D texture_colour;
layout (location = 7) uniform sampler2D texture_depth;

layout (location = 3) uniform int blur_intensity;

layout (location = 4) uniform int screen_width;
layout (location = 5) uniform int screen_height;


in vec2 texCoord;

vec3 blur()
{
	float dx = 1.0f/screen_width;
	float dy = 1.0f/screen_height;

	vec3 sum = vec3(0,0,0);

	for(int i = -blur_intensity; i < blur_intensity; i++)
	for(int j = -blur_intensity; j < blur_intensity; j++)
		sum += texture(texture_colour, texCoord+vec2(i * dx, j* dy)).xyz;
	return sum/(blur_intensity*blur_intensity*4);
}

void main()
{
	out_color = vec4(blur(), 1);
}