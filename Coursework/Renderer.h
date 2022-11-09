#pragma once
#include "../NCLGL/OGLRenderer.h"
#include "Scene.h"

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	 ~Renderer(void);
	 void RenderScene()				override;
	 void UpdateScene(float msec)	override;
protected:
	Scene* scene;
	std::vector<Shader*> shaders;

};
