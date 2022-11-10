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
protected:
	std::vector<Mesh*> geometries;	//change to Geometry type
	std::vector<GLuint> textures;
	std::vector<Shader*> shaders;
	Shader* currentShader = 0;		//0 is the 'null' object name for shader programs...

};

