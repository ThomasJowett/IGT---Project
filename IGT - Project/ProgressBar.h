#pragma once

#include "UIWidget.h"
#include "Mesh.h"
#include "Health.h"

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

class HealthBar :
	public ProgressBar, Observer<HealthEvent, GameObject*>
{
public:
	HealthBar(const char* name, Vector2D anchorPoint, Vector2D offset, const char* path, Vector2D size, float initialValue, GameObject* subject);

	void OnNotify(HealthEvent event, GameObject* gameObject)override;

private:
	Health* mHealth;
};