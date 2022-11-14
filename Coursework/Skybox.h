#pragma once
#include <nclgl/GeometryNode.h>

class Skybox : public GeometryNode {
public:
	Skybox(Mesh* mesh, Shader* shader) : GeometryNode("Skybox", mesh, shader) {}
	void Render() {
		glDepthMask(GL_FALSE);
		mesh->Draw();
		glDepthMask(GL_TRUE);
	}
	virtual void SendTextureToShader(Shader* s) {
		glUniform1i(glGetUniformLocation(s->GetProgram(), "cubeTex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	}
	void SetTexture(GLuint& cubeMapTexture) { this->cubeMapTexture = cubeMapTexture; }
protected:
	GLuint cubeMapTexture;
};