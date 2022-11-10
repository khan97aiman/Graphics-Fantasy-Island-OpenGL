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

void GroupNode::Update(float dt) {
	for (auto const& i : children) {
		i->Update(dt);
	}
}
