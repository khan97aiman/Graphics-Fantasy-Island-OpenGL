#pragma once
#include "Matrix4.h"
#include "Shader.h"

class GroupNode;
enum NodeType {
	CAMERA,
	LIGHT,
	DIRECTIONAL, //not liking this
	SPOT,
	POINTLIGHT,
	GEOMETRY,
	GROUP,
};

class SceneNode {
public:
	SceneNode() = default;
	SceneNode(std::string name): name(name) {}
	virtual ~SceneNode() {}
	void SetParent(GroupNode* parent) { this->parent = parent; }
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual NodeType GetNodeType() = 0;
	virtual void SendDataToShader(Shader* s, int index) = 0;
protected:
	GroupNode* parent = NULL;
	std::string name;
}; 

