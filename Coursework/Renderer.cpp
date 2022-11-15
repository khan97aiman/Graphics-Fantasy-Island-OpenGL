#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	scene = new Scene(width, height, parent.GetTimer());
	if (scene->GetBuildStatus() == FAILED) {
		return;
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	init = true;
}
Renderer::~Renderer(void)	{
	delete scene;
}

void Renderer::UpdateScene(float dt) {
	scene->Update(dt);
}

void Renderer::RenderScene()	{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->Render();
}

