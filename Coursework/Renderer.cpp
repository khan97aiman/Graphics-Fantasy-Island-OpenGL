#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	scene = new Scene();
	Shader* basicShader = new Shader("basicVertex.glsl", "colourFragment.glsl");
	if (!basicShader->LoadSuccess()) {
		return;
	}
	shaders.push_back(basicShader);

	init = true;
}
Renderer::~Renderer(void)	{
	delete scene;
	for (const auto& i : shaders) {
		delete i;
	}
}

void Renderer::UpdateScene(float dt) {

}

void Renderer::RenderScene()	{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	

	BindShader(shaders[0]);
	scene->Render();
}

