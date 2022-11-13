#pragma once
#include <nclgl/GeometryNode.h>

class Skybox : public GeometryNode {
public:
	Skybox(Mesh* mesh, Shader* shader) : GeometryNode("Skybox", mesh, shader) { }
	void Render() {
		glDepthMask(GL_FALSE);
		mesh->Draw();
		glDepthMask(GL_TRUE);
	}
};