#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	scene = new Scene();

	init = true;
}
Renderer::~Renderer(void)	{
	delete scene;
}

void Renderer::UpdateScene(float dt) {

}

void Renderer::RenderScene()	{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	
	scene->Render();
}

