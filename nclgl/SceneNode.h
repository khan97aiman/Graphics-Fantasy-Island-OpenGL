#pragma once
#include "Matrix4.h"

class SceneNode {
public:
	SceneNode() = default;
	SceneNode(std::string name): name(name) {}
	virtual ~SceneNode() {}
	void SetParent(SceneNode* parent) { this->parent = parent; }
protected:
	SceneNode* parent = NULL;
	std::string name;
};

