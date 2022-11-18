#pragma once
#include <nclgl/GeometryNode.h>

class Terrain : public GeometryNode {
public:
	Terrain(Mesh* mesh, Shader* shader) : GeometryNode("Terrain", Matrix4(), mesh, shader) {
		material = Material(Vector3(1.0f), Vector3(0.0f), 0.4f);
	}
	virtual void SendTextureToShader(Shader* s) {
		glUniform1i(glGetUniformLocation(s->GetProgram(), "mudTex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mudTexture);

		glUniform1i(glGetUniformLocation(s->GetProgram(), "grassTex"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		glUniform1i(glGetUniformLocation(s->GetProgram(), "rockTex"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, rockTexture);

		glUniform1i(glGetUniformLocation(s->GetProgram(), "snowTex"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, snowTexture);

		glUniform1i(glGetUniformLocation(s->GetProgram(), "bumpmap"), 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, bumpmap);
	}
	void SetTexture(GLuint& mudTexture, GLuint& rockTexture, GLuint& grassTexture, GLuint& snowTexture, GLuint& bumpmap) {
		this->mudTexture = mudTexture;
		this->rockTexture = rockTexture;
		this->grassTexture = grassTexture;
		this->snowTexture = snowTexture;
		this->bumpmap = bumpmap;
	}
protected:
	GLuint mudTexture;
	GLuint rockTexture;
	GLuint grassTexture;
	GLuint snowTexture;
	GLuint bumpmap;
};