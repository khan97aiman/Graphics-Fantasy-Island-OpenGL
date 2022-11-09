#pragma once
#include <nclgl/GroupNode.h>
#include <nclgl/Mesh.h>

class Scene : public GroupNode {
public:
	Scene(int width, int height);
	~Scene();
	void BindShader(Shader* s);
	void Render();
	void Update(float dt);
	//virtual NodeType GetNodeType() { return GROUP; }
protected:
	//GroupNode* world;
	std::vector<Mesh*> geometries; //change to Geometry type
	std::vector<GLuint> textures;
	std::vector<Shader*> shaders;
	Shader* currentShader;

};

