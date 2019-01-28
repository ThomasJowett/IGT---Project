#pragma once

#include "iComponents.h"
#include "Camera.h"

class CameraFollow : public iUpdateable
{
public:
	CameraFollow(GameObject* parent, Camera* camera);
	~CameraFollow();

	CameraFollow* Clone() override;
	void Update(float deltaTime) override;

private:
	Camera* mCameraRef;

	Vector3D mOffset;

	float mSmoothSpeed;
};