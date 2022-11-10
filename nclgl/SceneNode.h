#pragma once
#include "Matrix4.h"
#include "Shader.h"

enum NodeType {
	CAMERA,
	LIGHT,
	GEOMETRY,
	GROUP,
};

class SceneNode {
public:
	SceneNode() = default;
	SceneNode(std::string name): name(name) {}
	virtual ~SceneNode() {}
	void SetParent(SceneNode* parent) { this->parent = parent; }
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual NodeType GetNodeType() = 0;
protected:
	SceneNode* parent = NULL;
	std::string name;
}; 

