#pragma once
#include <nclgl/GeometryNode.h>

class Terrain : public GeometryNode {
public:
	Terrain(Mesh* mesh, Shader* shader) : GeometryNode("Terrain", Matrix4(), mesh, shader) {
		hasMaterial = true;
		material = Material(Vector3(1.0f), Vector3(0.0f), 0.4f);
	}
	virtual void SendTextureToShader(Shader* s) {
		glUniform1i(glGetUniformLocation(s->GetProgram(), "diffuseTex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, earthTexture);
	}
	void SetTexture(GLuint& texture) { this->earthTexture = texture; }
protected:
	GLuint earthTexture;
};