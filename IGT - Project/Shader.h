#ifndef _SHADER_H
#define _SHADER_H

#include <string>

#include "ShaderLoader.h"
#include "Matrix.h"

enum
{
	MODEL_U,
	VIEW_U,
	PROJECTION_U,
	TEXTURE_U,
	DEPTH_U,
	TINT_COLOUR_U,
	SCREEN_WIDTH_U,
	SCREEN_HEIGHT_U,

	NUM_UNIFORMS
};

class Shader
{
public:
	~Shader() {
		UnBind();
		glDeleteProgram(mProgram);
	}
	void Bind() { glUseProgram(mProgram); }
	void UnBind() { glUseProgram(0); }

	void UpdateMatrixUniform(int uniform, const Matrix4x4& matrix, bool transpose);
	void Updatefloat4(float r, float g, float b, float a);
	void UpdateInteger(int uniform, int value);
	void UpdateVector2(int uniform, Vector2D vector);

	GLuint GetTextureUnit(int uniform);

protected:
	GLuint mProgram;
	GLuint mUniforms[NUM_UNIFORMS];
};

class BasicShader : public Shader
{
public:
	BasicShader();
	~BasicShader() { }
private:

};

class GUIShader : public Shader
{
public:
	GUIShader();
	~GUIShader() { }
private:
};

class BlurShader : public Shader
{
public:
	BlurShader();
	~BlurShader() {}
};

class NoPostProcessShader : public Shader
{
public:
	NoPostProcessShader();
	~NoPostProcessShader() {}
};

class SplitScreenShader : public Shader
{
public:
	SplitScreenShader();
	~SplitScreenShader() {}
};
#endif // !_SHADER_H