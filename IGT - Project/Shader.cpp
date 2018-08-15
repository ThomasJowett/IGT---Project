#include "Shader.h"

void Shader::UpdateMatrixUniform(int uniform, const Matrix4x4 & matrix, bool transpose)
{
	glUniformMatrix4fv(mUniforms[uniform], 1, transpose, &matrix.m[0][0]);
}

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

GUIShader::GUIShader()
{
	mProgram = ShaderLoader::LoadShaderProgram("Shaders/GUIShader");

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3D), (void*)0);
	glEnableVertexAttribArray(0);

	//TexCoord
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2D), (void*)0);
	glEnableVertexAttribArray(1);

	//Uniforms
	mUniforms[MODEL_U] = glGetUniformLocation(mProgram, "model");
	mUniforms[PROJECTION_U] = glGetUniformLocation(mProgram, "projection");

	//Texture Samplers
	mUniforms[TEXTURE_U] = glGetUniformLocation(mProgram, "Texture");
	glUniform1i(mUniforms[TEXTURE_U], 0);
}
