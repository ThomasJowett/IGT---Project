#ifndef _CAMERA_H
#define _CAMERA_H

#include "Transform.h"
#include "Matrix.h"
#include "Shader.h"

class Camera
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

	int GetOrthoWidth() const { return mOrthoWidth; }
	int GetOrthoHeight() const { return mOrthoHeight; }

	Projection GetProjectionMethod() const { return mProjectionMethod; }

private:

	Transform * mTransform;
	Matrix4x4 mProjectionMatrix;
	Matrix4x4 mView;

	int mOrthoWidth;
	int mOrthoHeight;

	Projection mProjectionMethod;

	float mNearDepth;
	float mFarDepth;
};

#endif //_CAMERA_H