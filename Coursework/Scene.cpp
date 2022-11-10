#include "Scene.h"
#include <nclgl/PerspectiveCamera.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/GeometryNode.h>


Scene::Scene(int width, int height) {
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

	SceneNode* camera = new PerspectiveCamera(0.0f, 0.0f, Vector3(0.0f, 0.0f, 0.0f), 1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	
	AddChild(camera);


	Vector3 tempPos1 = Vector3(0, 0, -1500.0f);
	Vector3 tempPos2 = Vector3(-100, -100, -1000.0f);

	float scale = 100.0f;
	float rotation = 0.0f;

	SceneNode* triangleGeometry1 = new GeometryNode("Triangle1", Matrix4::Translation(tempPos1)* Matrix4::Rotation(rotation, Vector3(0, 1, 0))* Matrix4::Scale(Vector3(scale, scale, scale)) , geometries[0], basicShader1);
	AddChild(triangleGeometry1);

	SceneNode* triangleGeometry2 = new GeometryNode("Triangle2", Matrix4::Translation(tempPos2)* Matrix4::Rotation(rotation, Vector3(0, 1, 0))* Matrix4::Scale(Vector3(scale, scale, scale)) , geometries[0], basicShader2);
	AddChild(triangleGeometry2);
}

Scene::~Scene() {
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

void Scene::BindShader(Shader* s) {
	currentShader = s;
	glUseProgram(s->GetProgram());
}


void Scene::Render() {
	for (auto const& i : children) {
		if (i->GetNodeType() == GEOMETRY) {
			BindShader(dynamic_cast<GeometryNode*>(i)->GetShader());
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, dynamic_cast<GeometryNode*>(i)->GetModelMatrix().values);
			for (auto const& i : children) {
				if (i->GetNodeType() == CAMERA) {
					glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, dynamic_cast<Camera*>(i)->GetViewMatrix().values);
					glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, dynamic_cast<Camera*>(i)->GetProjectionMatrix().values);

				}
			}
		}
		i->Render();
	}
}

void Scene::Update(float dt) {
	for (auto const& i : children) {
		i->Update(dt);
	}
}