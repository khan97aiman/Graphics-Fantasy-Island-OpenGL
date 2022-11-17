#pragma once
#include <nclgl/GeometryNode.h>

class Water : public GeometryNode {
public:
	Water(Mesh* mesh, Shader* shader, Vector3 size) : GeometryNode("Water", mesh, shader) {
		transform =
		Matrix4::Translation(size * Vector3(0.5f, 0.5f, 1.0f) + Vector3(0, -95, -100)) *
			Matrix4::Scale(size * Vector3(1.0f, 0.5f, 1.0f)) *
			Matrix4::Rotation(-105, Vector3(1, 0, 0));

	}
	virtual void SendTextureToShader(Shader* s) {
		glUniform1i(glGetUniformLocation(s->GetProgram(), "cubeTex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);


		glUniform1i(glGetUniformLocation(s->GetProgram(), "waterTex"), 1); 
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterTexture);

		glUniform1i(glGetUniformLocation(s->GetProgram(), "dudvMap"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterTexture);

	}
	void SetTexture(GLuint& cubeMapTexture, GLuint& waterTexture, GLuint& dudvMap) {
		this->cubeMapTexture = cubeMapTexture;
		this->waterTexture = waterTexture;
		this->dudvMap = dudvMap;
	}
protected:
	GLuint cubeMapTexture;
	GLuint waterTexture;
	GLuint dudvMap;
};