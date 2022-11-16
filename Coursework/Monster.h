#pragma once
#include <nclgl/MeshAnimation.h>
#include <nclgl/GeometryNode.h>

class Monster : public GeometryNode {
public:
	Monster(Mesh* mesh, MeshAnimation* animation, Shader* shader) : GeometryNode("Monster", Matrix4(), mesh, shader), animation(animation) {}
	virtual void SendTextureToShader(Shader* s) {
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
		for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		}
	}
	virtual void Update(float dt) {
		frameTime -= dt;
		while (frameTime < 0.0f) {
			currentFrame = (currentFrame + 1) % animation->GetFrameCount();
			frameTime += 1.0f / animation->GetFrameRate();
		}
		GeometryNode::Update(dt);
	}
	void Render() {
		vector <Matrix4 > frameMatrices;
		const Matrix4 * invBindPose = mesh->GetInverseBindPose();
		const Matrix4 * frameData = animation->GetJointData(currentFrame);
		for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
			frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
		}
		int j = glGetUniformLocation(shader->GetProgram(), "joints");
		glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());
		
		for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
			mesh->DrawSubMesh(i);
		}
	}
	void SetTexture(vector <GLuint>& matTextures) {
		this->matTextures = matTextures;
	}
protected:
	vector <GLuint> matTextures;
	MeshAnimation* animation;
	int currentFrame = 0;
	float frameTime = 0.0f;
};