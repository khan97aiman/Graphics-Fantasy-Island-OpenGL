#include "GeometryNode.h"

void GeometryNode::Update(float dt) {
	//if you want to rotate translate a geometry, use dt in calculations
	UpdateWorldTransform();
	modelMatrix = worldTransform * Matrix4::Scale(modelScale);
	normalMatrix = Matrix3(modelMatrix).Inverse().Transposed();
	//if geometry node also has children, call update and render of all its children too
}

void GeometryNode::SendDataToShader(Shader* s, int index) {
	glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
	glUniformMatrix3fv(glGetUniformLocation(s->GetProgram(), "normalMatrix"), 1, false, normalMatrix.values);

	if (hasTextureMatrix) {
		glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "textureMatrix"), 1, false, textureMatrix.values);
	}
	if (hasMaterial) {
		material.SendDataToShader(s);
	}
	SendTextureToShader(s);
}

void GeometryNode::Render() {
	mesh->Draw();
}
