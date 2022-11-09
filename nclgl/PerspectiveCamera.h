#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera() = default;
	PerspectiveCamera(float pitch, float yaw, Vector3 position, float znear, float zfar, float aspect, float fov) : Camera(pitch, yaw, position) {
		this->znear = znear;
		this->zfar = zfar;
		this->aspect = aspect;
		this->fov = fov;
		BuildProjectionMatrix();
	};
	virtual ~PerspectiveCamera(void) {};
	virtual void BuildProjectionMatrix() { projectionMatrix = Matrix4::Perspective(znear, zfar, aspect, fov); }

protected:
	float znear = 0.0f;
	float zfar = 0.0f;
	float aspect = 0.0f;
	float fov = 0.0f;
};
