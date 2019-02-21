#version 460 core

in VertexData{
	vec3 posW;
	vec2 texCoord;
} FragmentIn;

out vec4 FragColour;

layout (location = 3) uniform sampler2D Texture;

layout (location = 4) uniform vec4 TintColour;

void main()
{
	vec4 textureColour = texture2D(Texture, FragmentIn.texCoord);
	if(textureColour.a < 0.2)
		discard;
	FragColour = textureColour * TintColour;
}