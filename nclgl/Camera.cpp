#include "Camera.h"
#include "Window.h"

Camera::Camera(float pitch, float yaw, Vector3 position) {
	this->pitch = pitch;
	this->yaw = yaw;
	this->position = position;
	this->startingYaw = yaw;
}

void Camera::Update(float dt) {
	UpdateCameraFromUserInput(dt);
	BuildViewMatrix();
	frameFrustum.BuildFustrum(projectionMatrix * viewMatrix);
}

void Camera::BuildViewMatrix() {
	viewMatrix = Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
}

void Camera::SendDataToShader(Shader* s, int index) {
	glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "viewMatrix"), 1, false, viewMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "projMatrix"), 1, false, projectionMatrix.values);
	glUniform3fv(glGetUniformLocation(s->GetProgram(), "cameraPos"), 1, (float*)&position);
}

void Camera::UpdateCameraFromUserInput(float dt) {

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));
	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);
	float speed = 500.0f * dt;

	if (!animate) {

		pitch -= (Window::GetMouse()->GetRelativePosition().y);
		yaw -= (Window::GetMouse()->GetRelativePosition().x);
		pitch = std::min(pitch, 90.0f);
		pitch = std::max(pitch, -90.0f);
		if (yaw < 0) {
			yaw += 360.0f;
		}
		if (yaw > 360.0f) {
			yaw -= 360.0f;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
			position += forward * speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
			position -= forward * speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
			position -= right * speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
			position += right * speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
			position.y += speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
			position.y -= speed;
		}
	}
	else {
		if (cycle == 0) {
			yaw += 0.25;
			if (yaw - startingYaw > 360) {
				yaw = startingYaw;
				cycle++;
			}
		}
		else if (cycle == 1) {
			yaw += 0.1;
			position += forward * speed ;
			position.y += speed/10;
			if (yaw - startingYaw > 60) {
				cycle++;
			}
		}
		else if (cycle == 2) {
			yaw += 0.25;
			if (yaw > 360) {
				cycle++;
			}			
		}
		else if (cycle == 3) {
			pitch += 0.025;
			position -= forward * speed;
			position.y -= speed/10;
			if (position.y < 1000) {
				cycle++;
			}
		}
		else
			animate = false;
	}
}
