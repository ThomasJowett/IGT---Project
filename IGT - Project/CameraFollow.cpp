#include "CameraFollow.h"

CameraFollow::CameraFollow(GameObject * parent, Camera * camera)
	:iUpdateable(parent), mCameraRef(camera)
{
	mSmoothSpeed = 1.0f;
	mOffset = Vector3D(0,0,0);
}

CameraFollow::~CameraFollow()
{
}

CameraFollow * CameraFollow::Clone()
{
	return new CameraFollow(nullptr, mCameraRef);
}

void CameraFollow::Update(float deltaTime)
{
	Vector3D desiredPosition = GetParent()->GetWorldTransform().mPosition + mOffset;

	Vector3D smoothedPosition = Vector3D::Lerp(mCameraRef->GetTransform()->mPosition, desiredPosition, mSmoothSpeed * deltaTime);

	mCameraRef->GetTransform()->mPosition.x = smoothedPosition.x;
	mCameraRef->GetTransform()->mPosition.y = smoothedPosition.y;

	//mCameraRef->GetTransform()->mPosition = smoothedPosition;
}
