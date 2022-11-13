#include "Scene.h"
#include <nclgl/PerspectiveCamera.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/GeometryNode.h>
#include <nclgl/HeightMap.h>


Scene::Scene(int width, int height) : width(width), height(height) {
	LoadShaders();
	LoadGeometries();
	LoadTextures();
	
	AddLights();	// Lights
	AddCamera();	// Camera
	AddObjects();	// Action!
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
	// no need to delete children here, group node's destructor will handle it
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
			glUniformMatrix3fv(glGetUniformLocation(currentShader->GetProgram(), "normalMatrix"), 1, false, dynamic_cast<GeometryNode*>(i)->GetNormalMatrix().values);
			for (auto const& i : children) {
				if (i->GetNodeType() == CAMERA) {
					glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, dynamic_cast<Camera*>(i)->GetViewMatrix().values);
					glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, dynamic_cast<Camera*>(i)->GetProjectionMatrix().values);
					glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&dynamic_cast<Camera*>(i)->GetPosition());

				}
			}
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numDirectionalLights"), 0);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numSpotLights"), 0); //change these numbers to vector size
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numPointLights"), 1);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0); //handle texture in geometry class
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);

			for (int i = 0; i < directionalLights.size(); i++) {
				directionalLights[i]->SendDataToShader(currentShader, i);
			}

			for (int i = 0; i < pointLights.size(); i++) {
				pointLights[i]->SendDataToShader(currentShader, i);
			}

			for (int i = 0; i < spotLights.size(); i++) {
				spotLights[i]->SendDataToShader(currentShader, i);
			}

			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "material.ambient"), 1, (float*)&Vector3(0.24725,0.1995, 0.0745));
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "material.diffuse"), 1, (float*)&Vector3(0.75164, 0.60648, 0.22648));
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "material.specular"), 1, (float*)&Vector3(0.628281, 0.555802, 0.366065));
			glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "material.shininess"), 0.4);

		}
		i->Render();
	}
}

void Scene::Update(float dt) {
	UpdateWorldTransform();
	for (auto const& i : children) {
		i->Update(dt);
	}
}

void Scene::LoadShaders() {
	shaders.push_back(new Shader("matrixVertex.glsl", "lightsColourFragment.glsl"));

	for (const auto& shader : shaders) {
		if (!shader->LoadSuccess()) {
			return;
		}
	}
}

void Scene::LoadGeometries() {
	geometries.push_back(new HeightMap(TEXTUREDIR"noise1.png"));
}

void Scene::LoadTextures() {
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	for (const auto& texture : textures) {
		if (!texture) {
			return;
		}
		SetTextureRepeating(texture, true);
	}
	
	buildStatus = SUCCESS;
}

void Scene::AddCamera() {
	AddChild(new PerspectiveCamera(-40, 270, Vector3(2048, 510, 2048), 1.0f, 10000.0f, (float)width / (float)height, 45.0f));
}

void Scene::AddLights() {
	directionalLights.push_back(new DirectionalLight(Vector3(0, -1, 0)));
	AddChild(directionalLights.back());

	pointLights.push_back(new PointLight(Vector3(2048, 153, 2048)));
	AddChild(pointLights.back());

	spotLights.push_back(new SpotLight(Vector3(2048, 53, 2048), Vector3(-1, -1, 0), 45.0f));
	AddChild(spotLights.back());
}

void Scene::AddObjects() {	
	GeometryNode* triangleGeometry1 = new GeometryNode("Terrain", Matrix4(), geometries[0], shaders[0]);
	AddChild(triangleGeometry1);
}

void Scene::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}
