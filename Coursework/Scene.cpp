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

			if (day) {
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numDirectionalLights"), directionalLights.size());
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numSpotLights"), 0); //change these numbers to vector size
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numPointLights"), 0);
			}
			else {
				//Sending Number of Lights
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numDirectionalLights"), 0);
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numSpotLights"), spotLights.size()); //change these numbers to vector size
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numPointLights"), pointLights.size());
			}
			
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
	//std::cout << "pitch: " << currentCamera->GetPitch() << " yaw: " << currentCamera->GetYaw() << " position: " << currentCamera->GetPosition() << std::endl;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
		day = !day;
	}
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

	Mesh* treeMesh = Mesh::LoadFromMeshFile("tree1a.msh");
	geometries.push_back(treeMesh);
	MeshMaterial* treeMaterial = new MeshMaterial("tree1a.mat");
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
	currentCamera = new PerspectiveCamera(-25, 100, Vector3(11742, 922.375, 465.768), 1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	AddChild(currentCamera);
}

void Scene::AddLights() {
	directionalLights.push_back(new DirectionalLight(Vector3(-1, -1, 0)));
	AddChild(directionalLights.back());

	PointLight* p1 = new PointLight(dimensions * Vector3(0.25f, 2.0f, 0.5f));
	p1->SetIntensity(4.0f);
	pointLights.push_back(p1);
	AddChild(pointLights.back());

	PointLight* p2 = new PointLight(dimensions * Vector3(0.8f, 3.0f, 0.1f));
	p2->SetIntensity(4.0f);
	pointLights.push_back(p2);
	AddChild(pointLights.back());

	PointLight* p3 = new PointLight(dimensions * Vector3(0.2f, 3.0f, 0.1f));
	p3->SetIntensity(4.0f);
	pointLights.push_back(p3);
	AddChild(pointLights.back());

	PointLight* p4 = new PointLight(dimensions * Vector3(0.5f, 3.0f, 0.5f));
	p4->SetIntensity(1.0f);
	p4->SetDiffuseColor(Vector3(135, 206, 235) / 255);
	pointLights.push_back(p4);
	AddChild(pointLights.back());

	SpotLight* s1 = new SpotLight(dimensions * Vector3(0.4f, 5.0f, 0.3f), Vector3(1, -1, 0), 45.0f);
	s1->SetIntensity(2.0f);
	s1->SetDiffuseColor(Vector3(255, 170, 51) / 255);
	spotLights.push_back(s1);
	AddChild(spotLights.back());

	SpotLight* s2 = new SpotLight(dimensions * Vector3(0.3f, 5.0f, 0.3f), Vector3(1, -1, 0), 45.0f);
	s2->SetIntensity(2.0f);
	s2->SetDiffuseColor(Vector3(127, 255, 212) / 255);
	spotLights.push_back(s2);
	AddChild(spotLights.back());

	SpotLight* s3 = new SpotLight(dimensions * Vector3(0.6f, 5.0f, 0.3f), Vector3(1, 0, 1), 45.0f);
	s3->SetIntensity(2.0f);
	s3->SetDiffuseColor(Vector3(135, 206, 235) / 255);
	spotLights.push_back(s3);
	AddChild(spotLights.back());

	SpotLight* s4 = new SpotLight(dimensions * Vector3(0.4f, 5.0f, 0.5f), Vector3(1, 0, 1), 45.0f);
	s4->SetIntensity(4.0f);
	s4->SetDiffuseColor(Vector3(135, 206, 235) / 255);
	spotLights.push_back(s4);
	AddChild(spotLights.back());

	SpotLight* s5 = new SpotLight(dimensions * Vector3(0.5f, 5.0f, 0.5f), Vector3(-1, 0, 1), 45.0f);
	s5->SetIntensity(4.0f);
	s5->SetDiffuseColor(Vector3(135, 206, 235) / 255);
	spotLights.push_back(s5);
	AddChild(spotLights.back());
}

void Scene::AddObjects() {	
	Skybox* skybox = new Skybox(geometries[0], shaders[0]);
	skybox->SetTexture(textures[0]);
	AddChild(skybox);

	for (int i = 0; i < 25; i++) {
		Tree* tree = new Tree(geometries[4], shaders[4]);
		tree->SetTexture(skeletalTextures[1]);
		int scale = 80;
		tree->SetTransform(Matrix4::Translation(Vector3(dimensions.x * 1.25 - 500 * i, 700, dimensions.z - dimensions.z / 1.45)) * Matrix4::Scale(Vector3(scale, scale, scale)));
		AddChild(tree);
	}

	for (int i = 0; i < 25; i++) {
		Tree* tree = new Tree(geometries[4], shaders[4]);
		tree->SetTexture(skeletalTextures[1]);
		int scale = 80;
		tree->SetTransform(Matrix4::Translation(Vector3(dimensions.x * 1.25 - 500 * 24, 700, dimensions.z - dimensions.z / 1.45 + 500*i)) * Matrix4::Scale(Vector3(scale, scale, scale)));
		AddChild(tree);
	}

	for (int i = 0; i < 25; i++) {
		Tree* tree = new Tree(geometries[4], shaders[4]);
		tree->SetTexture(skeletalTextures[1]);
		int scale = 80;
		tree->SetTransform(Matrix4::Translation(Vector3(dimensions.x * 2.2 - 500 * 24, 700, dimensions.z - dimensions.z / 1.45 + 500 * i)) * Matrix4::Scale(Vector3(scale, scale, scale)));
		AddChild(tree);
	}

	Terrain* terrain = new Terrain(geometries[1], shaders[1]);
	terrain->SetTexture(textures[2], textures[3], textures[4], textures[5]);
	AddChild(terrain);

	Monster* monster = new Monster(geometries[3], meshAnimations[0], shaders[3]);
	monster->SetTexture(skeletalTextures[0]);
	monster->SetTransform(Matrix4::Translation(Vector3(dimensions.x / 2, 1, dimensions.z / 4)) * Matrix4::Scale(Vector3(200, 200, 200)));
	AddChild(monster);

	Monster* monster2 = new Monster(geometries[3], meshAnimations[0], shaders[3], CLOCKWISE);
	monster2->SetTexture(skeletalTextures[0]);
	monster2->SetTransform(Matrix4::Translation(Vector3(dimensions.x / 3, 1, dimensions.z / 4)) * Matrix4::Scale(Vector3(400, 400, 400)) * Matrix4::Rotation(180, Vector3(0, 1, 0)));
	AddChild(monster2);



	Water* water = new Water(geometries[0], shaders[2], dimensions);
	water->SetTexture(textures[0], textures[1], textures[6]);
	AddChild(water);

}

void Scene::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}
