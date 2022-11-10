#include "GroupNode.h"
#include "GeometryNode.h"
#include "Camera.h"

GroupNode::~GroupNode() {
	for (const auto& i: children) {
		delete i;
	}
}

void GroupNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->SetParent(this);
}


void GroupNode::Render() {
	for (auto const& i : children) {
		i->Render();
	}
}

/*
	* If this node has a parent, then the world transform is world * local,
	* Otherwise, its a root node , and world transform is local transform!
*/
void GroupNode::UpdateWorldTransform() {
	parent ? worldTransform = parent->worldTransform * transform : worldTransform = transform;
}

void GroupNode::Update(float dt) {
	UpdateWorldTransform();
	for (auto const& i : children) {
		i->Update(dt);
	}
}
