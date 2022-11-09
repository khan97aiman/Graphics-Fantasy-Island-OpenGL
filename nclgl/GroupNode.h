#pragma once
#include <vector>
#include "SceneNode.h"

/*
* This node can either serve as a transition node or a group node to a collection of geometries
*/

class GroupNode : public SceneNode {
public:
	GroupNode() = default;
	GroupNode(std::string name) : SceneNode(name) {}
	GroupNode(std::string name, Matrix4 worldTransform, Matrix4 transform) : SceneNode(name), worldTransform(worldTransform), transform(transform) {}
	virtual ~GroupNode();
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }
	void AddChild(SceneNode* s);
	virtual void Update(float dt) {};
	virtual void Render();
protected:
	std::vector<SceneNode*> children;
	Matrix4 worldTransform;
	Matrix4 transform;
};
