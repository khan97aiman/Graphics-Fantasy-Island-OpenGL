#pragma once
#include "Matrix4.h"
#include "Frustum.h"
#include "SceneNode.h"

class Camera : public SceneNode {
public:
	Camera() = default;
	Camera(float pitch, float yaw, Vector3 position);
	virtual ~Camera(void) {};
	void Update(float dt);
	void Render() {}
	void BuildViewMatrix();
	virtual void BuildProjectionMatrix() = 0;
	NodeType GetNodeType() { return CAMERA; }
	virtual void SendDataToShader(Shader* s, int index = 0);
	/*Vector3 GetPosition() { return position; }
	float GetPitch() { return pitch; }
	float GetYaw() { return yaw; }*/
protected:
	void UpdateCameraFromUserInput(float dt);
	float yaw = 0.0f;
	float pitch = 0.0f;
	Vector3 position = Vector3(0.0f, 0.0f, 0.0f); 
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	Frustum frameFrustum;
};

