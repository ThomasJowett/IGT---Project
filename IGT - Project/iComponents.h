#pragma once

class iUpdateable
{
public:
	virtual void Update(float deltaTime) = 0;
};

class iRenderable
{
public:
	virtual void Render() = 0;
};