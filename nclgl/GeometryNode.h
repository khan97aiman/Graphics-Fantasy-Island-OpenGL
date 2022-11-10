#pragma once
#include "Vector3.h"
#include "Shader.h"
#include "GroupNode.h"
#include "Matrix3.h"

class Geometry; //change all Mesh types to Geometry

class GeometryNode : public GroupNode {
public:
	GeometryNode() = default;
	GeometryNode(std::string name) : GroupNode(name) {}
	GeometryNode(std::string name, Mesh* mesh) : GroupNode(name), mesh(mesh) {}
	GeometryNode(std::string name, Matrix4 transform, Mesh* mesh, Shader* s) : GroupNode(name, transform), mesh(mesh), shader(s) {}
	virtual ~GeometryNode() {}
	void SetModelScale(Vector3 s) { this->modelScale = s; }
	void SetColour(Vector4 c) { this->colour = c; }
	void SetTexture(GLuint tex) { this->texture = tex; }
	void SetShader(Shader* shader) { this->shader = shader; }
	void SetGeometry(Mesh* mesh) { this->mesh = mesh; }
	float GetBoundingRadius() const { return boundingRadius; }
	Matrix4 GetModelMatrix() { return modelMatrix; }
	Matrix4 GetNormalMatrix() { return modelMatrix; }
	Shader* GetShader() { return shader; }
	virtual NodeType GetNodeType() { return GEOMETRY; }
	virtual void Render();
	virtual void Update(float dt);	
protected:
	Mesh* mesh = NULL;
	Vector3 modelScale = Vector3(1, 1, 1);
	Matrix4 modelMatrix;
	Matrix3 normalMatrix;
	float distanceFromCamera = 0.0f;
	float boundingRadius = 1.0f;
	GLuint texture = 0; 
	Vector4 colour = Vector4(1, 1, 1, 1);
	Shader* shader = 0;						//0 is the 'null' object name for shader programs...

};

