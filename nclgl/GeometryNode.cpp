#include "GeometryNode.h"

void GeometryNode::Update(float dt) {
	//if you want to rotate translate a geometry, use dt in calculations
	UpdateWorldTransform();
	modelMatrix = worldTransform * Matrix4::Scale(modelScale);
	//if geometry node also has children, call update and render of all its children too, and delete all its children in the destructor
}

void GeometryNode::Render() {
	mesh->Draw();
}
