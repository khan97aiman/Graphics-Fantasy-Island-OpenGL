#include "GeometryNode.h"

void GeometryNode::Update(float dt) {
	//if you want to rotate translate a geometry, use dt in calculations
	UpdateWorldTransform();
	modelMatrix = worldTransform * Matrix4::Scale(modelScale);
	normalMatrix = Matrix3(modelMatrix).Inverse().Transposed();
	//if geometry node also has children, call update and render of all its children too
}

void GeometryNode::Render() {
	mesh->Draw();
}
