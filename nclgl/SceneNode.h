#pragma once
#include "Matrix4.h"
#include "Shader.h"

class SceneNode {
public:
	SceneNode() = default;
	SceneNode(std::string name): name(name) {}
	virtual ~SceneNode() {}
	void SetParent(SceneNode* parent) { this->parent = parent; }
	virtual void Render() = 0;
	//virtual void Animate() = 0;
	virtual void Update(float dt) = 0;
	void SetShader(Shader* shader) {
		this->shader = shader;
	}
	void BindShader() {
		glUseProgram(shader->GetProgram());
	}
protected:
	SceneNode* parent = NULL;
	std::string name;
	Shader* shader = 0;	//0 is the 'null' object name for shader programs...

}; 

