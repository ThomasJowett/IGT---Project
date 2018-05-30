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

	void Orthographic(Vector3D position, float width, float hieght, float nearDepth, float farDepth);
	void Perspective(Vector3D position, Vector3D forward, Vector3D up, float fovY, float nearDepth, float farDepth);
	void Update(Shader* shader);

	Matrix4x4 GetView() const { return mView; }
	Matrix4x4 GetProjection() const { return mProjection; }

	Transform* GetTransform() { return mTransform; }

private:
	Transform * mTransform;
	Matrix4x4 mProjection;
	Matrix4x4 mView;
};

#endif //_CAMERA_H