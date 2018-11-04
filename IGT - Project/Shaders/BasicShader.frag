#version 460 core

in VertexData{
	vec3 posW;
	vec2 texCoord;
} FragmentIn;

out vec4 FragColour;

uniform sampler2D Texture;

uniform vec4 TintColour;

void main()
{
	vec4 textureColour = texture2D(Texture, FragmentIn.texCoord);
	if(textureColour.a < 0.2)
		discard;
	FragColour = textureColour * TintColour;
}