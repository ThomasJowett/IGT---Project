#include "Shader.h"
#include "Settings.h"
#include <iostream>

void Shader::UpdateMatrixUniform(int uniform, const Matrix4x4 & matrix, bool transpose)
{
	glUniformMatrix4fv(mUniforms[uniform], 1, transpose, &matrix.m[0][0]);
}

void Shader::Updatefloat4(float r, float g, float b, float a)
{
	glUniform4f(mUniforms[TINT_COLOUR_U], r, g, b, a);
}

void Shader::UpdateInteger(int uniform, int value)
{
	glUniform1i(mUniforms[uniform], value);
}

void Shader::UpdateVector2(int uniform, Vector2D vector)
{
	glUniform2f(mUniforms[uniform], vector.x, vector.y);
}

GLuint Shader::GetTextureUnit(int uniform)
{
	return mUniforms[uniform];
}



BasicShader::BasicShader()
{
	mProgram = ShaderLoader::LoadShaderProgram("Shaders/BasicShader");
	Bind();
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

	mUniforms[TINT_COLOUR_U] = glGetUniformLocation(mProgram, "TintColour");
	glUniform4f(mUniforms[TINT_COLOUR_U], 1.0f, 1.0f, 1.0f, 1.0f);

	UnBind();
}

GUIShader::GUIShader()
{
	mProgram = ShaderLoader::LoadShaderProgram("Shaders/GUIShader");
	Bind();
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
	UnBind();
}

BlurShader::BlurShader()
{
	mProgram = ShaderLoader::LoadShaderProgram("Shaders/PassThroughShader", "Shaders/BlurShader");

	Bind();
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3D), (void*)0);
	glEnableVertexAttribArray(0);

	//TexCoord
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2D), (void*)0);
	glEnableVertexAttribArray(1);

	//Texture Samplers
	mUniforms[TEXTURE_U] = glGetUniformLocation(mProgram, "texture_colour");
	glUniform1i(mUniforms[TEXTURE_U], 0);

	mUniforms[DEPTH_U] = glGetUniformLocation(mProgram, "texture_depth");
	glUniform1i(mUniforms[DEPTH_U], 1);

	mUniforms[TINT_COLOUR_U] = glGetUniformLocation(mProgram, "blur_intensity");
	glUniform1i(mUniforms[TINT_COLOUR_U], 1);

	//uniforms
	mUniforms[SCREEN_WIDTH_U] = glGetUniformLocation(mProgram, "screen_width");
	glUniform1i(mUniforms[SCREEN_WIDTH_U], Settings::GetInstance()->GetScreenWidth());

	mUniforms[SCREEN_HEIGHT_U] = glGetUniformLocation(mProgram, "screen_height");
	glUniform1i(mUniforms[SCREEN_HEIGHT_U], Settings::GetInstance()->GetScreenHeight());
	UnBind();
}

NoPostProcessShader::NoPostProcessShader()
{
	mProgram = ShaderLoader::LoadShaderProgram("Shaders/PassThroughShader", "Shaders/NoPostProcess");

	Bind();

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3D), (void*)0);
	glEnableVertexAttribArray(0);

	//TexCoord
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2D), (void*)0);
	glEnableVertexAttribArray(1);

	//Texture Samplers
	mUniforms[TEXTURE_U] = glGetUniformLocation(mProgram, "texture_colour");
	glUniform1i(mUniforms[TEXTURE_U], 0);

	UnBind();
}

SplitScreenShader::SplitScreenShader()
{
	mProgram = ShaderLoader::LoadShaderProgram("Shaders/PassThroughShader", "Shaders/SplitScreen");

	Bind();

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3D), (void*)0);
	glEnableVertexAttribArray(0);

	//TexCoord
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2D), (void*)0);
	glEnableVertexAttribArray(1);

	//Texture Samplers
	mUniforms[TEXTURE_U] = glGetUniformLocation(mProgram, "player1_camera");
	glUniform1i(mUniforms[TEXTURE_U], 0);

	mUniforms[4] = glGetUniformLocation(mProgram, "player2_camera");
	glUniform1i(mUniforms[4], 1);
	
	//Camera locations
	mUniforms[6] = glGetUniformLocation(mProgram, "player1_location");
	glUniform2f(mUniforms[6], 0.0f, 0.0f);
	
	mUniforms[7] = glGetUniformLocation(mProgram, "player2_location");
	glUniform2f(mUniforms[7], 0.0f, 0.0f);

	UnBind();
}
