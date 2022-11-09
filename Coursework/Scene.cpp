#include "Scene.h"
#include <nclgl/PerspectiveCamera.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/GeometryNode.h>


Scene::Scene() {
	geometries.push_back(Mesh::GenerateTriangle());
	world = new GroupNode("World");
	SceneNode* triangleGeometry = new GeometryNode("Triangle", geometries[0]);
	world->AddChild(triangleGeometry);
}

Scene::~Scene() {
	delete world;
	for (const auto& i : geometries) {
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