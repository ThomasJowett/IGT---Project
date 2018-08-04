#include "Shader.h"

BasicShader::BasicShader()
{
	mProgram = ShaderLoader::LoadShaderProgram("Shaders/BasicShader");

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3D), (void*)0);
	glEnableVertexAttribArray(0);

	//TexCoord
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2D), (void*)0);
	glEnableVertexAttribArray(1);

	//Uniforms
	mUniforms[MODEL_U] = glGetUniformLocation(mProgram, "model");
	mUniforms[VIEW_U] = glGetUniformLocation(mProgram, "view");
	mUniforms[PROJECTION_U] = glGetUniformLocation(mProgram, "projection");

	//Texture Samplers
	mUniforms[TEXTURE_U] = glGetUniformLocation(mProgram, "Texture");
	glUniform1i(mUniforms[TEXTURE_U], 0);
}

void Shader::UpdateWorld(const Matrix4x4 world)
{
	glUniformMatrix4fv(mUniforms[MODEL_U], 1, GL_TRUE, &world.m[0][0]);
}

void Shader::UpdateViewProjection(const Matrix4x4 & view, const Matrix4x4 & projection)
{
	glUniformMatrix4fv(mUniforms[VIEW_U], 1, GL_FALSE, &view.m[0][0]);

	glUniformMatrix4fv(mUniforms[PROJECTION_U], 1, GL_FALSE, &projection.m[0][0]);
}
