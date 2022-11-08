#pragma once
#include "Matrix4.h"
#include "Frustum.h"
class Camera {
public:
	Camera() = default;
	Camera(float pitch, float yaw, Vector3 position);
	virtual ~Camera(void) {};
	void UpdateCamera(float dt);
	void BuildViewMatrix();
	virtual void BuildProjectionMatrix() = 0;
protected:
	void UpdateCameraFromUserInput(float dt);

	float yaw = 0.0f;
	float pitch = 0.0f;
	Vector3 position = Vector3(0.0f, 0.0f, 0.0f); 
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	Frustum frameFrustum;
};
