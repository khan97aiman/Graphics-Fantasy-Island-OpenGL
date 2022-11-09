#pragma once
#include "Vector3.h"
#include "Shader.h"
#include "GroupNode.h"

class Geometry; //change all Mesh types to Geometry

class GeometryNode : public GroupNode {
public:
	GeometryNode() = default;
	GeometryNode(std::string name, Mesh* mesh) : GroupNode(name), mesh(mesh) {}
	GeometryNode(std::string name, Matrix4 worldTransform, Matrix4 transform, Mesh* mesh) : GroupNode(name, worldTransform, transform), mesh(mesh) {}
	void SetModelScale(Vector3 s) { modelScale = s; }
	void SetColour(Vector4 c) { colour = c; }
	void SetTexture(GLuint tex) { texture = tex; }
	float GetBoundingRadius() const { return boundingRadius; }
	virtual void Update(float dt) {};
	virtual void Render();
protected:
	Mesh* mesh = NULL;
	Vector3 modelScale = Vector3(1, 1, 1);
	Matrix4 modelMatrix;
	float distanceFromCamera = 0.0f;
	float boundingRadius = 1.0f;
	GLuint texture = 0; 
	Vector4 colour = Vector4(1, 1, 1, 1);
};

