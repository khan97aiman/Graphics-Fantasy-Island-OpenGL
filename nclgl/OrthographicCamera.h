#pragma once
#include "Camera.h"

class OrthographicCamera : public Camera {
public:
	OrthographicCamera() = default;
	OrthographicCamera(float pitch, float yaw, Vector3 position, float znear, float zfar, float right, float left, float top, float bottom) : Camera(pitch, yaw, position) {
		this->znear = znear;
		this->zfar = zfar;
		this->right = right;
		this->left = left;
		this->top = top;
		this->bottom = bottom;
		BuildProjectionMatrix();
	};
	virtual ~OrthographicCamera(void) {};
	virtual void BuildProjectionMatrix() { projectionMatrix = Matrix4::Orthographic(znear, zfar, right, left, top, bottom); }

protected:
	float znear = 0.0f;
	float zfar = 0.0f;
	float right = 0.0f;
	float left = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
};
