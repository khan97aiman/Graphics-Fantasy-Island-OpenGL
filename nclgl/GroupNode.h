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
	GroupNode(std::string name, Matrix4 transform) : SceneNode(name), transform(transform) {}
	virtual ~GroupNode();
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }
	void SetTransform(const Matrix4& matrix) { transform = matrix; }
	void AddChild(SceneNode* s);
	virtual void Update(float dt);
	virtual void Render();
	virtual NodeType GetNodeType() { return GROUP; }
protected:
	std::vector<SceneNode*> children;
	Matrix4 worldTransform;
	Matrix4 transform;
};
