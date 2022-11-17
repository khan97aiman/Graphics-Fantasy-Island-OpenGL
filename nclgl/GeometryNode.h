#pragma once
#include "Vector3.h"
#include "Shader.h"
#include "GroupNode.h"
#include "Matrix3.h"
#include "Material.h"

class Geometry; //change all Mesh types to Geometry

class GeometryNode : public GroupNode {
public:
	GeometryNode() = default;
	GeometryNode(std::string name) : GroupNode(name) {}
	GeometryNode(std::string name, Mesh* mesh) : GroupNode(name), mesh(mesh) {}
	GeometryNode(std::string name, Mesh* mesh, Shader* s) : GroupNode(name), mesh(mesh), shader(s) {}
	GeometryNode(std::string name, Matrix4 transform, Mesh* mesh, Shader* s) : GroupNode(name, transform), mesh(mesh), shader(s) {}
	virtual ~GeometryNode() {}
	void SetModelScale(Vector3 s) { this->modelScale = s; }
	float GetBoundingRadius() const { return boundingRadius; }
	Shader* GetShader() { return shader; }
	virtual NodeType GetNodeType() { return GEOMETRY; }
	virtual void Render();
	virtual void Update(float dt);	
	virtual void SendDataToShader(Shader* s, int index);
	virtual void SendTextureToShader(Shader* s) {}
protected:
	Mesh* mesh = NULL;
	Vector3 modelScale = Vector3(1, 1, 1);
	Matrix4 modelMatrix;
	Matrix3 normalMatrix;
	Matrix3 textureMatrix;
	float distanceFromCamera = 0.0f;
	float boundingRadius = 1.0f;

	Material material;
	Shader* shader = 0;						//0 is the 'null' object name for shader programs...
};

