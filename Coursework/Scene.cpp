#include "Scene.h"
#include <nclgl/PerspectiveCamera.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/GeometryNode.h>


Scene::Scene() {
	geometries.push_back(Mesh::GenerateTriangle());
	Shader* basicShader1 = new Shader("basicVertex.glsl", "colourFragment.glsl");
	if (!basicShader1->LoadSuccess()) {
		return;
	}
	shaders.push_back(basicShader1);
	Shader* basicShader2 = new Shader("basicVertex.glsl", "colourFragmentConstant.glsl");
	if (!basicShader2->LoadSuccess()) {
		return;
	}
	shaders.push_back(basicShader2);

	world = new GroupNode("World");
	SceneNode* triangleGeometry1 = new GeometryNode("Triangle1", geometries[0]);
	triangleGeometry1->SetShader(basicShader1);
	world->AddChild(triangleGeometry1);

	SceneNode* triangleGeometry2 = new GeometryNode("Triangle2", geometries[0]);
	triangleGeometry2->SetShader(basicShader2);
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
}

//write shaders first, display only 1 triangle with camera and lights
//complete render and draw funcs
//complete renderer class