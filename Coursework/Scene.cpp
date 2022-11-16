#include "Scene.h"
#include <nclgl/PerspectiveCamera.h>
#include <nclgl/DirectionalLight.h>
#include <nclgl/GeometryNode.h>
#include <nclgl/HeightMap.h>
#include "Terrain.h"
#include "Skybox.h"
#include "Water.h"
#include "Monster.h"
#include "Tree.h"


Scene::Scene(int width, int height, GameTimer* gameTImer) : width(width), height(height), gameTImer(gameTImer) {
	LoadShaders();
	LoadGeometries();
	LoadTextures();
	LoadSkeletons();
	
	AddLights();	// Lights
	AddCamera();	// Camera
	AddObjects();	// Action!
}

Scene::~Scene() {
	for (const auto& i : geometries) {
		delete i;
	}
	for (const auto& i : meshMaterials) {
		delete i;
	}
	for (const auto& i : meshAnimations) {
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
			
			glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"), gameTImer->GetTotalTimeSeconds());


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
	shaders.push_back(new Shader("skyboxVertex.glsl", "skyboxFragment.glsl"));
	shaders.push_back(new Shader("matrixVertex.glsl", "lightsColourFragment.glsl"));
	shaders.push_back(new Shader("reflectVertex.glsl", "reflectFragment.glsl"));
	shaders.push_back(new Shader("skinningVertex.glsl", "texturedFragment.glsl"));
	shaders.push_back(new Shader("nonSkinningVertex.glsl", "texturedFragment.glsl"));

	for (const auto& shader : shaders) {
		if (!shader->LoadSuccess()) {
			return;
		}
	}
}

void Scene::LoadGeometries() {
	geometries.push_back(Mesh::GenerateQuad());
	HeightMap* terrainHeightMap = new HeightMap(TEXTUREDIR"noise5-big.jpg");
	HeightMap* waterHeightMap = new HeightMap(TEXTUREDIR"sea-noise1.jpg", true);
	dimensions = terrainHeightMap->GetHeightmapSize();
	//transform = Matrix4::Translation(Vector3(-dimensions.x / 2, 1, -dimensions.z / 2));
	geometries.push_back(terrainHeightMap);
	geometries.push_back(waterHeightMap);
}

void Scene::LoadTextures() {
	textures.push_back(
		SOIL_load_OGL_cubemap(
			TEXTUREDIR"posx.jpg", TEXTUREDIR"negx.jpg",
			TEXTUREDIR"posy.jpg", TEXTUREDIR"negy.jpg",
			TEXTUREDIR"posz.jpg", TEXTUREDIR"negz.jpg",
			SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0
		)
	);
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"mud.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"mountain-rock.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"grass.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"snow-grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	textures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"water-dudv.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));



	for (auto& texture = textures.begin() + 1; texture != textures.end(); ++texture) {
		if (!*texture) {
			return;
		}
		SetTextureRepeating(*texture, true);
	}
	
	buildStatus = SUCCESS;
}

void Scene::LoadSkeletons() {
	Mesh* mesh = Mesh::LoadFromMeshFile("Role_T.msh");
	geometries.push_back(mesh);
	MeshMaterial* meshMaterial = new MeshMaterial("Role_T.mat");
	meshMaterials.push_back(meshMaterial);
	meshAnimations.push_back(new MeshAnimation("Role_T.anm"));

	std::vector<GLuint> temp;
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = meshMaterial->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		textures.emplace_back(texID);
		temp.emplace_back(texID);
	}
	skeletalTextures.push_back(temp);


	Mesh* treeMesh = Mesh::LoadFromMeshFile("Tree10_4.msh");
	geometries.push_back(treeMesh);
	MeshMaterial* treeMaterial = new MeshMaterial("Tree10_4.mat");
	meshMaterials.push_back(treeMaterial);

	temp.clear();
	for (int i = 0; i < treeMesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = treeMaterial->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		textures.emplace_back(texID);
		temp.emplace_back(texID);
	}
	skeletalTextures.push_back(temp);
}
void Scene::AddCamera() {
	currentCamera = new PerspectiveCamera(-45, 0.0f, (dimensions * Vector3(0.5, 10.0f, 0.5f)), 1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	AddChild(currentCamera);
}

void Scene::AddLights() {
	directionalLights.push_back(new DirectionalLight(Vector3(-1, -1, 0)));
	AddChild(directionalLights.back());

	pointLights.push_back(new PointLight(dimensions * Vector3(0.5f, 1.0f, 0.5f)));
	AddChild(pointLights.back());

	spotLights.push_back(new SpotLight(dimensions * Vector3(0.5f, 5.0f, 0.5f), Vector3(0, -1, 0), 45.0f));
	AddChild(spotLights.back());
}

void Scene::AddObjects() {	
	Skybox* skybox = new Skybox(geometries[0], shaders[0]);
	skybox->SetTexture(textures[0]);
	AddChild(skybox);

	Terrain* terrain = new Terrain(geometries[1], shaders[1]);
	terrain->SetTexture(textures[2], textures[3], textures[4], textures[5]);
	AddChild(terrain);

	Monster* monster = new Monster(geometries[3], meshAnimations[0], shaders[3]);
	monster->SetTexture(skeletalTextures[0]);
	monster->SetTransform(Matrix4::Translation(Vector3(dimensions.x / 2, 1, dimensions.z / 4)) * Matrix4::Scale(Vector3(500, 500, 500)));
	AddChild(monster);

	

	Water* water = new Water(geometries[0], shaders[2], dimensions);
	water->SetTexture(textures[0], textures[1], textures[6]);
	AddChild(water);

	for (int i = 0; i < 25; i++) {
		Tree* tree = new Tree(geometries[4], shaders[4]);
		tree->SetTexture(skeletalTextures[1]);
		int scale = (rand() % 50) + 100;
		tree->SetTransform(Matrix4::Translation(Vector3(dimensions.x - 500*i, 450, dimensions.z - 800 + (rand() % 800))) * Matrix4::Scale(Vector3(scale, scale, scale)));
		AddChild(tree);
	}

}

void Scene::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}
