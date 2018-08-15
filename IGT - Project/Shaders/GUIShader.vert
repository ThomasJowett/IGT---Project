#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out VertexData{
	vec3 posW;
	vec2 texCoord;
}VertexOut;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	VertexOut.texCoord = texCoord;
	vec4 posW = model * vec4(position.xy, 0.0, 1.0);
	VertexOut.posW = posW.xyz;
	gl_Position = projection * posW;
}