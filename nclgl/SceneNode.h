#pragma once
#include "Matrix4.h"

class SceneNode {
public:
	SceneNode() = default;
	SceneNode(std::string name): name(name) {}
	virtual ~SceneNode() {}
	void SetParent(SceneNode* parent) { this->parent = parent; }
	virtual void Render() = 0;
	//virtual void Animate() = 0;
	virtual void Update(float dt) = 0;
protected:
	SceneNode* parent = NULL;
	std::string name;
}; 

