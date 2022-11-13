#include "Scene.h"
#include <nclgl/PerspectiveCamera.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/GeometryNode.h>
#include <nclgl/HeightMap.h>
#include "Terrain.h"
#include "Skybox.h"


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
			
			//Sending GeometryNodes Data 
			i->SendDataToShader(currentShader, 0);
	
			//Sending Camera Matrices
			currentCamera->SendDataToShader(currentShader);

			//Sending Number of Lights
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numDirectionalLights"), 1);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numSpotLights"), 0); //change these numbers to vector size
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numPointLights"), 0);

			//Sending Texture Data
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0); //handle texture in geometry class
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textures[1]);

			for (int i = 0; i < directionalLights.size(); i++) {
				directionalLights[i]->SendDataToShader(currentShader, i);
			}

			for (int i = 0; i < pointLights.size(); i++) {
				pointLights[i]->SendDataToShader(currentShader, i);
			}

			for (int i = 0; i < spotLights.size(); i++) {
				spotLights[i]->SendDataToShader(currentShader, i);
			}

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
	shaders.push_back(new Shader("skyboxVertex.glsl", "skyboxFragment.glsl"));

	for (const auto& shader : shaders) {
		if (!shader->LoadSuccess()) {
			return;
		}
	}
}

void Scene::LoadGeometries() {
	geometries.push_back(new HeightMap(TEXTUREDIR"noise1.png"));
	geometries.push_back(Mesh::GenerateQuad());
}

void Scene::LoadTextures() {
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	textures.push_back(
		SOIL_load_OGL_cubemap(
			TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
			TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
			TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
			SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0
		)
	);
	for (const auto& texture : textures) {
		if (!texture) {
			return;
		}
		SetTextureRepeating(texture, true);
	}
	
	buildStatus = SUCCESS;
}

void Scene::AddCamera() {
	currentCamera = new PerspectiveCamera(-40, 270, Vector3(2048, 510, 2048), 1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	AddChild(currentCamera);
}

void Scene::AddLights() {
	directionalLights.push_back(new DirectionalLight(Vector3(1, -1, 0)));
	AddChild(directionalLights.back());

	pointLights.push_back(new PointLight(Vector3(2048, 153, 2048)));
	AddChild(pointLights.back());

	spotLights.push_back(new SpotLight(Vector3(2048, 53, 2048), Vector3(-1, -1, 0), 45.0f));
	AddChild(spotLights.back());
}

void Scene::AddObjects() {	
	Skybox* skybox = new Skybox(geometries[1], shaders[1]);
	AddChild(skybox);

	Terrain* terrain = new Terrain(geometries[0], shaders[0]);
	AddChild(terrain);
}

void Scene::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}
