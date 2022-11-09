#include "GeometryNode.h"

void GeometryNode::Render() {
	BindShader();
	mesh->Draw();
}
