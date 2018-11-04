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
	TINT_COLOUR_U,

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
#endif // !_SHADER_H