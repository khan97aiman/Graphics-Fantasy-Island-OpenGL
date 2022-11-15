#pragma once
#include <nclgl/GeometryNode.h>

class Water : public GeometryNode {
public:
	Water(Mesh* mesh, Shader* shader, Vector3 size) : GeometryNode("Water", mesh, shader) {
		hasTextureMatrix = true;

		transform =
		/*	Matrix4::Translation(Vector3(-size.x / 2, 250, -size.z / 2)) *
			Matrix4::Scale(2.0f) *
			Matrix4::Rotation(0.95, Vector3(1, 0, 0));*/
		Matrix4::Translation(size * 0.5f + Vector3(0, -100, 0)) *
			Matrix4::Scale(size * 1.0f) *
			Matrix4::Rotation(-75, Vector3(1, 0, 0));

	}
	void Update(float dt) {
		waterRotate += dt * 1.0f; //2 degrees a second
		waterCycle += dt * 0.025f; //10 units a second
		/*textureMatrix =
			Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
			Matrix4::Scale(Vector3(10, 10, 10)) *
			Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));*/

		GeometryNode::Update(dt);
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
	float waterRotate = 0.0f;
	float waterCycle = 0.0f;
	GLuint cubeMapTexture;
	GLuint waterTexture;
	GLuint dudvMap;
};