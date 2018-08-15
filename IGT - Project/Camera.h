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

	void Orthographic(float OrthoWidth, float OrthoHieght, float nearDepth, float farDepth);
	void Perspective(Vector3D forward, Vector3D up, float fovY, float nearDepth, float farDepth);
	void UpdateView(Shader* shader);

	Matrix4x4 GetView() const { return mView; }
	Matrix4x4 GetProjection() const { return mProjection; }

	Transform* GetTransform() { return mTransform; }

private:
	Transform * mTransform;
	Matrix4x4 mProjection;
	Matrix4x4 mView;

	int mOrthoWidth;
	int mOrthoHeight;
};

#endif //_CAMERA_H