#pragma once
#include <nclgl/GroupNode.h>
#include <nclgl/Mesh.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/PointLight.h>
#include <nclgl/SpotLight.h>
#include <nclgl/Camera.h>

enum Build {
	FAILED,
	SUCCESS
};

class Scene : public GroupNode {
public:
	Scene(int width, int height, GameTimer* gameTImer);
	virtual ~Scene();
	void BindShader(Shader* s);
	void Render();
	void Update(float dt);
	void LoadShaders();
	void LoadGeometries();
	void LoadTextures();
	void AddLights();
	void AddCamera();
	void AddObjects();
	int GetBuildStatus() { return buildStatus; }
	void SetTextureRepeating(GLuint target, bool repeating);
protected:
	std::vector<Mesh*> geometries;	// change to Geometry type
	std::vector<GLuint> textures;
	std::vector<Shader*> shaders;
	std::vector<DirectionalLight*> directionalLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;
	Camera* currentCamera;
	Shader* currentShader = 0;		// 0 is the 'null' object name for shader programs...
	int width;						// rendering width required for aspect ratio of camera
	int height;						// rendering height required for aspect ratio of camera
	Build buildStatus = FAILED;
	Vector3 dimensions;
	GameTimer* gameTImer = NULL;
};

