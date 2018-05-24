#ifndef _SHADER_H
#define _SHADER_H

#include <string>

#include "ShaderLoader.h"
#include "Transform.h"
#include "Matrix.h"

enum
{
	MODEL_U,
	VIEW_U,
	PROJECTION_U,
	TEXTURE_U,

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

	void UpdateWorld(const Transform* transform);
	void UpdateViewProjection(const Matrix4x4& view, const Matrix4x4& projection);
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
#endif // !_SHADER_H