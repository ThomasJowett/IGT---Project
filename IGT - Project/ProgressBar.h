#pragma once

#include "UIWidget.h"
#include "Mesh.h"

class ProgressBar :
	public UIWidget
{
public:
	ProgressBar(const char* name, Vector2D anchorPoint, Vector2D offset, const char* path, Vector2D size, float initialValue);
	void SetPercent(float value);

	void Render(Shader* shader) override;
private:
	void UpdateBar();
	float mCurrentValue;
	float mIncrementStep;

	GLuint mTextureID;
	Mesh* mMesh;

	Vector2D mSize;
};