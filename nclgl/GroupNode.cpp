#include "GroupNode.h"

GroupNode::~GroupNode() {
	for (const auto& i: children) {
		delete i;
	}
}

void GroupNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->SetParent(this);
}
