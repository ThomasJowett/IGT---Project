#version 460 core

layout(location = 0) out vec4 out_color;

layout(location = 6) uniform sampler2D player1_camera;
layout(location = 7) uniform sampler2D player2_camera;

layout(location = 4) uniform vec2 player1_location;
layout(location = 5) uniform vec2 player2_location;

in vec2 texCoord;

void main()
{
	vec2 pixelLocation = vec2((player1_location.x + player2_location.x)/2 , (player1_location.y + player2_location.y)/2)
		+ texCoord - vec2(0.5f, 0.5f);

	float distanceTo1 = distance(pixelLocation, player1_location);
	float distanceTo2 = distance(pixelLocation, player2_location);

	if(distanceTo1 < distanceTo2)
	{
		out_color = texture(player1_camera, texCoord);
	}
	else
	{
		out_color = texture(player2_camera, texCoord);
	}

	if((distanceTo1 - distanceTo2) > -0.003f && (distanceTo1 - distanceTo2) < 0.003f)
		out_color = vec4(0.17f, 0.13f, 0.125f, 1.0f);
}