#ifndef _ICOMPONENTS_H

#define _ICOMPONENTS_H

#include "Shader.h"

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;
};

class iUpdateable : public Component
{
public:
	
	virtual void Update(float deltaTime) = 0;
};

class iRenderable : public Component
{
public:
	virtual void Render(Shader* shader) = 0;
};

#endif // !_ICOMPONENTS_H