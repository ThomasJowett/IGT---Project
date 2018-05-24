#pragma once
#include "Commons.h"
#include <string>

class ShaderLoader
{
public:
	static GLuint LoadShaderProgram(const std::string filename);
};