#pragma once
#include <nclgl/GeometryNode.h>

class Tree : public GeometryNode {
public:
	Tree(Mesh* mesh, Shader* shader) : GeometryNode("Tree", Matrix4(), mesh, shader) {
		hasMaterial = true;
		material = Material(Vector3(124, 252, 0) / 255, Vector3(124, 252, 0)/255, 32);
	}
	virtual void SendTextureToShader(Shader* s) {
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
		for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		}
	}
	void Render() {
		for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
			mesh->DrawSubMesh(i);
		}
	}
	void SetTexture(vector <GLuint>& matTextures) {
		this->matTextures = matTextures;
	}
protected:
	vector <GLuint> matTextures;
};