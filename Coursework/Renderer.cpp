#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	scene = new Scene(width, height);
	if (scene->GetBuildStatus() == FAILED) {
		return;
	}
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
	glClear(GL_COLOR_BUFFER_BIT);	
	scene->Render();
}

