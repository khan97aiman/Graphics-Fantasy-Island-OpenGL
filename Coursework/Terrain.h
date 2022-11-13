#pragma once
#include <nclgl/GeometryNode.h>

class Terrain : public GeometryNode {
protected:
	GLuint earthTexture;
public:
	Terrain(Mesh* mesh, Shader* shader) : GeometryNode("Terrain", Matrix4(), mesh, shader) {
		material = Material(Vector3(1.0f), Vector3(0.0f), 0.4f);
	}
};