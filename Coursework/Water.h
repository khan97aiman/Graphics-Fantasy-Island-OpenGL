#pragma once
#include <nclgl/GeometryNode.h>

class Water : public GeometryNode {
public:
	Water(Mesh* mesh, Shader* shader, Vector3 size) : GeometryNode("Water", mesh, shader) {
		hasTextureMatrix = true;

		transform =
			Matrix4::Translation(size * 0.5f) *
			Matrix4::Scale(size * 1.0f) *
			Matrix4::Rotation(-90, Vector3(1, 0, 0));

	}
	void Update(float dt) {
		waterRotate += dt * 1.0f; //2 degrees a second
		waterCycle += dt * 0.025f; //10 units a second
		textureMatrix =
			Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
			Matrix4::Scale(Vector3(10, 10, 10)) *
			Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

		GeometryNode::Update(dt);
	}
protected:
	float waterRotate = 0.0f;
	float waterCycle = 0.0f;
};