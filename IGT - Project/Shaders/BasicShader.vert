#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out VertexData{
	vec3 posW;
	vec2 texCoord;
}VertexOut;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;

void main()
{
	VertexOut.texCoord = texCoord;
	vec4 posW = model * vec4(position, 1.0);
	VertexOut.posW = posW.xyz;
	gl_Position = projection * view * posW;
}