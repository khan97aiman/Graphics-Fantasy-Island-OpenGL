#pragma once
#include "Vector3.h"
#include "Shader.h"
#include "GroupNode.h"

class Geometry;

class GeometryNode : public GroupNode {
public:
	GeometryNode() = default;
	//GeometryNode(std::string name, Matrix4 worldTransform, Matrix4 transform, Geometry geometry) : GroupNode(name, worldTransform, transform), geometry(geometry) {}
	void SetModelScale(Vector3 s) { modelScale = s; }
	void SetColour(Vector4 c) { colour = c; }
	void SetTexture(GLuint tex) { texture = tex; }
	void Draw();
protected:
	Geometry* geometry = NULL;
	Vector3 modelScale = Vector3(1, 1, 1);
	Shader* shader;
	Matrix4 modelMatrix;
	float distanceFromCamera = 0.0f;
	float boundingRadius = 1.0f;
	GLuint texture = 0; 
	Vector4 colour = Vector4(1, 1, 1, 1);
};

