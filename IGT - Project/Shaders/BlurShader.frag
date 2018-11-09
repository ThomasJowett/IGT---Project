#version 460 core

layout(location = 0) out vec4 out_color;

layout (location = 6) uniform sampler2D texture_colour;
layout (location = 7) uniform sampler2D texture_depth;

layout (location = 3) uniform int c;

layout (location = 4) uniform int screen_width;
layout (location = 5) uniform int screen_height;


in vec2 texCoord;

vec3 blur()
{
	float dx = 1.0f/screen_width;
	float dy = 1.0f/screen_height;

	vec3 sum = vec3(0,0,0);

	for(int i = -5; i < 5; i++)
	for(int j = -5; j < 5; j++)
		sum += texture(texture_colour, texCoord+vec2(i * dx, j* dy)).xyz;
	return sum/100;
}

void main()
{
	//if(c==0) out_color = vec4(blur(), 1);
	//else if(c==1) out_color = texture(texture_depth, texCoord);
	//else if(c==2) out_color = texture(texture_colour, texCoord);

	if(texCoord.y <0.5f)
		out_color = texture(texture_depth, texCoord);
	else
		out_color = vec4(blur(), 1);
}