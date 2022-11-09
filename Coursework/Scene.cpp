#include "Scene.h"
#include <nclgl/PerspectiveCamera.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/GeometryNode.h>


Scene::Scene() {
	geometries.push_back(Mesh::GenerateTriangle());
	Shader* basicShader1 = new Shader("matrixVertex.glsl", "colourFragment.glsl");
	if (!basicShader1->LoadSuccess()) {
		return;
	}
	shaders.push_back(basicShader1);
	Shader* basicShader2 = new Shader("matrixVertex.glsl", "colourFragmentConstant.glsl");
	if (!basicShader2->LoadSuccess()) {
		return;
	}
	shaders.push_back(basicShader2);

	world = new GroupNode("World");

	SceneNode* camera = new PerspectiveCamera(0.0f, 0.0f, Vector3(0.0f, 0.0f, 0.0f), 1.0f, 10000.0f, (float)800 / (float)600, 45.0f);
	
	world->AddChild(camera);


	Vector3 tempPos1 = Vector3(0, 0, -1500.0f);
	Vector3 tempPos2 = Vector3(-100, -100, -1000.0f);

	float scale = 100.0f;
	float rotation = 0.0f;

	SceneNode* triangleGeometry1 = new GeometryNode("Triangle1", Matrix4(), Matrix4::Translation(tempPos1)* Matrix4::Rotation(rotation, Vector3(0, 1, 0))* Matrix4::Scale(Vector3(scale, scale, scale)) , geometries[0], basicShader1);
	world->AddChild(triangleGeometry1);

	SceneNode* triangleGeometry2 = new GeometryNode("Triangle2", Matrix4(), Matrix4::Translation(tempPos2)* Matrix4::Rotation(rotation, Vector3(0, 1, 0))* Matrix4::Scale(Vector3(scale, scale, scale)) , geometries[0], basicShader2);
	world->AddChild(triangleGeometry2);
}

Scene::~Scene() {
	delete world;
	for (const auto& i : geometries) {
		delete i;
	}
	for (const auto& i : shaders) {
		delete i;
	}
	for (const auto& i : textures) {
		glDeleteTextures(1, &i);
	}
}

void Scene::Render() {
	world->Render();
}

void Scene::Update(float dt) {
	//call update of all nodes
	world->Update(dt);
}

//write shaders first, display only 1 triangle with camera and lights
//complete render and draw funcs
//complete renderer class