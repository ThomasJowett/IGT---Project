#include "Camera.h"

#include "Settings.h"

Camera::Camera()
{
	mTransform = new Transform();
}


Camera::~Camera()
{
}

void Camera::Orthographic(float OrthoWidth, float OrthoHeight, float nearDepth, float farDepth)
{
	mOrthoWidth = OrthoWidth;
	mOrthoHeight = OrthoHeight;

	mProjection = Matrix4x4::Orthographic(-(OrthoWidth / 2), (OrthoWidth / 2), -(OrthoHeight / 2), (OrthoHeight / 2), nearDepth, farDepth);
}

void Camera::Perspective(Vector3D forward, Vector3D up, float fovY, float nearDepth, float farDepth)
{
	mProjection = Matrix4x4::Perspective(fovY, (float)Settings::GetInstance()->GetScreenWidth() / Settings::GetInstance()->GetScreenHeight(),
		nearDepth, farDepth);
}

void Camera::UpdateView(Shader * shader)
{
	//mOrthoWidth++;
	//mOrthoHeight++;
	//mProjection = Matrix4x4::Orthographic(-(mOrthoWidth / 2), (mOrthoWidth / 2), -(mOrthoHeight / 2), (mOrthoHeight / 2), 0, 1000);

	mView = Matrix4x4::LookAt(mTransform->mPosition, mTransform->mPosition + Vector3D(0, 0, -1), Vector3D(0, 1, 0));
	shader->UpdateMatrixUniform(VIEW_U, mView,false);
	shader->UpdateMatrixUniform(PROJECTION_U, mProjection,false);
}
