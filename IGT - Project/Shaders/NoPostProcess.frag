#version 460 core

layout(location = 0) out vec4 out_color;

layout(location = 6) uniform sampler2D texture_colour;

in vec2 texCoord;

void main()
{
	out_color = texture(texture_colour, texCoord);
}
