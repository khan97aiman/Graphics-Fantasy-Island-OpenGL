#pragma once
#include <nclgl/GroupNode.h>
#include <nclgl/Mesh.h>

class Scene : public SceneNode {
public:
	Scene();
	~Scene();
	void Render();
	void Update(float dt);
protected:
	GroupNode* world;
	std::vector<Mesh*> geometries; //change to Geometry type
	std::vector<GLuint> textures;
	std::vector<Shader*> shaders;
};

