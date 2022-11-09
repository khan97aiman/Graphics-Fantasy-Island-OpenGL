#pragma once
#include "Vector3.h"
#include "Shader.h"
#include "GroupNode.h"

class Geometry; //change all Mesh types to Geometry

class GeometryNode : public GroupNode {
public:
	GeometryNode() = default;
	GeometryNode(std::string name, Mesh* mesh) : GroupNode(name), mesh(mesh) {}
	GeometryNode(std::string name, Matrix4 worldTransform, Matrix4 transform, Mesh* mesh, Shader* s) : GroupNode(name, worldTransform, transform), mesh(mesh), shader(s) {}
	void SetModelScale(Vector3 s) { modelScale = s; }
	void SetColour(Vector4 c) { colour = c; }
	void SetTexture(GLuint tex) { texture = tex; }
	float GetBoundingRadius() const { return boundingRadius; }
	virtual void Update(float dt) {
		//modelMatrix = worldTransform * transform * Matrix4::Scale(modelScale);
		modelMatrix = transform;
	};
	Matrix4 GetModelMatrix() { return modelMatrix; }
	virtual void Render();
	void SetShader(Shader* shader) {
		this->shader = shader;
	}
	Shader* GetShader() { return shader; }
	virtual NodeType GetNodeType() { return GEOMETRY; }

	//void BindShader() {
	//	glUseProgram(shader->GetProgram());
	//}
protected:
	Mesh* mesh = NULL;
	Vector3 modelScale = Vector3(1, 1, 1);
	Matrix4 modelMatrix;
	float distanceFromCamera = 0.0f;
	float boundingRadius = 1.0f;
	GLuint texture = 0; 
	Vector4 colour = Vector4(1, 1, 1, 1);
	Shader* shader = 0;	//0 is the 'null' object name for shader programs...

};

