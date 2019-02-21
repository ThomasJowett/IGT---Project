#include "Camera.h"



Camera::Camera()
{
	mTransform = new Transform();
	Settings::GetInstance()->AddObserver(this);
}


Camera::~Camera()
{
	Settings::GetInstance()->RemoveObserver(this);
}

void Camera::Orthographic(float OrthoWidth, float OrthoHeight, float nearDepth, float farDepth)
{
	mProjectionMethod = ORTHOGRAPHIC;
	mOrthoWidth = OrthoWidth;
	mOrthoHeight = OrthoHeight;
	mNearDepth = nearDepth;
	mFarDepth = farDepth;

	mProjectionMatrix = Matrix4x4::Orthographic(-(OrthoWidth / 2), (OrthoWidth / 2), -(OrthoHeight / 2), (OrthoHeight / 2), nearDepth, farDepth);
}

void Camera::Perspective(Vector3D forward, Vector3D up, float fovY, float nearDepth, float farDepth)
{
	mProjectionMethod = PERSPECTIVE;
	mNearDepth = nearDepth;
	mFarDepth = farDepth;

	mProjectionMatrix = Matrix4x4::Perspective(fovY, (float)Settings::GetInstance()->GetScreenWidth() / Settings::GetInstance()->GetScreenHeight(),
		nearDepth, farDepth);
}

void Camera::UpdateView(Shader * shader)
{
	mView = Matrix4x4::LookAt(mTransform->mPosition, mTransform->mPosition + Vector3D(0, 0, -1), Vector3D(0, 1, 0));
	shader->UpdateMatrixUniform(VIEW_U, mView,false);
	shader->UpdateMatrixUniform(PROJECTION_U, mProjectionMatrix,false);
}

void Camera::OnNotify(SettingsEvent event, Vector2D data)
{
	float scale = Settings::GetInstance()->GetScreenScale();

	switch (event)
	{
	case SettingsEvent::ON_RESOLUTION_CHANGE:
		switch (mProjectionMethod)
		{
		case Camera::PERSPECTIVE:
			//Perspective()
			break;
		case Camera::ORTHOGRAPHIC:
			
			Orthographic(data.x / scale, data.y / scale, mNearDepth, mFarDepth);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
