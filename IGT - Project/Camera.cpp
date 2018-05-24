#include "Camera.h"

#include "Constants.h"

Camera::Camera()
{
	mTransform = new Transform();
}


Camera::~Camera()
{
}

void Camera::Initialise(Vector3D position, float width, float hieght, float nearDepth, float farDepth)
{
	mTransform->mPosition = position;

	//mProjection = Matrix4x4::Perspective(60, (float)SCREEN_WIDTH / SCREEN_HEIGHT, nearDepth, farDepth);
	mProjection = Matrix4x4::Orthographic(-(width / 2), (width / 2), -(hieght / 2), (hieght / 2), nearDepth, farDepth);
}

void Camera::Update(Shader * shader)
{
	mView = Matrix4x4::LookAt(mTransform->mPosition, mTransform->mPosition + Vector3D(0, 0, -1), Vector3D(0, 1, 0));
	shader->UpdateViewProjection(mView, mProjection);
}
