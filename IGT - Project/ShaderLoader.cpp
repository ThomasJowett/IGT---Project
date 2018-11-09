#include "ShaderLoader.h"
#include <iostream>
#include <fstream>

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const char* errorMessage);
std::string LoadShader(const std::string& filename);
GLuint CreateShader(const std::string& text, GLenum shaderType);

GLuint ShaderLoader::LoadShaderProgram(const std::string filename)
{
	GLuint program = glCreateProgram();
	GLuint vertexShader = CreateShader(LoadShader(filename + ".vert"), GL_VERTEX_SHADER);
	GLuint fragmentShader = CreateShader(LoadShader(filename + ".frag"), GL_FRAGMENT_SHADER);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "ERROR: Program linking failed: ");

	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "ERROR: Program is invalid: ");

	return program;
}

GLuint ShaderLoader::LoadShaderProgram(const std::string vertexShaderFilename, const std::string fragmentShaderFilename)
{
	GLuint program = glCreateProgram();
	GLuint vertexShader = CreateShader(LoadShader(vertexShaderFilename + ".vert"), GL_VERTEX_SHADER);
	GLuint fragmentShader = CreateShader(LoadShader(fragmentShaderFilename + ".frag"), GL_FRAGMENT_SHADER);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "ERROR: Program linking failed: ");

	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "ERROR: Program is invalid: ");

	return program;
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const char* errorMessage)
{
	GLint success = 0;
	char error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": " << error << " " << std::endl;
	}
}

std::string LoadShader(const std::string& filename)
{
	std::ifstream file;
	file.open((filename).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << filename << std::endl;
	}

	return output;
}

GLuint CreateShader(const std::string & text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader creation failed!" << std::endl;

	const char* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

	return shader;
}
