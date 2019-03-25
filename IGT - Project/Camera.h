#ifndef _CAMERA_H
#define _CAMERA_H

#include "Transform.h"
#include "Matrix.h"
#include "Shader.h"
#include "Settings.h"

class Camera:public Observer<SettingsEvent, Vector2D>
{
public:
	Camera();
	~Camera();

	enum Projection
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	void Orthographic(float OrthoWidth, float OrthoHieght, float nearDepth, float farDepth);
	void Perspective(Vector3D forward, Vector3D up, float fovY, float nearDepth, float farDepth);
	void UpdateView(Shader* shader);

	Matrix4x4 GetView() const { return mView; }
	Matrix4x4 GetProjection() const { return mProjectionMatrix; }

	Transform* GetTransform() { return mTransform; }

	float GetNearDepth() const { return mNearDepth; }
	float GetFarDepth() const { return mFarDepth; }

	int GetOrthoWidth() const { return (int)mOrthoWidth; }
	int GetOrthoHeight() const { return (int)mOrthoHeight; }

	Projection GetProjectionMethod() const { return mProjectionMethod; }

	void OnNotify(SettingsEvent event, Vector2D data)override;

	Vector2D WorldTransformToScreenPosition(Transform* transform)const;

private:

	Transform * mTransform;
	Matrix4x4 mProjectionMatrix;
	Matrix4x4 mView;

	float mOrthoWidth;
	float mOrthoHeight;

	Projection mProjectionMethod = ORTHOGRAPHIC;

	float mNearDepth = 0.0f;
	float mFarDepth = 100.0f;
};

#endif //_CAMERA_H