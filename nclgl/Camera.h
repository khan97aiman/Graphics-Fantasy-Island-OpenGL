#pragma once
#include "Matrix4.h"
class Camera {
public:
	Camera() = default;
	Camera(float pitch, float yaw, Vector3 position);
	virtual ~Camera(void) {};
	void UpdateCamera(float dt = 1.0f);
	void BuildViewMatrix();
	virtual void BuildProjectionMatrix() = 0;
protected:
	float yaw = 0.0f;
	float pitch = 0.0f;
	Vector3 position = Vector3(0.0f, 0.0f, 0.0f); 
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
};

