#pragma once
#include "Shader.h"

class iUpdateable
{
public:
	virtual void Update(float deltaTime) = 0;
};

class iRenderable
{
public:
	virtual void Render(Shader* shader) = 0;
};