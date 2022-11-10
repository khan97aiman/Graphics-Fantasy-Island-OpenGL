#pragma once
#include "Vector3.h"
#include "Shader.h"
#include "SceneNode.h"
/*
* The diffuse intensity of a light is the aspect of the light that interacts with diffuse material color,  
* The specular intensity of a light is what interacts with specular material color.
* The ambient intensity of a light in OpenGL is added to the general level of ambient light.
* Read more here: https://math.hws.edu/graphicsbook/c4/s1.html
*/
class Light: public SceneNode {
public:
	Light() = default;
	Light(Vector3 colour) {
		this->colour = colour;
	}
	Light(float ambient, float diffuse, float specular, Vector3 colour) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->colour = colour;
	}
	void Update(float dt) {}
	void Render() {}

	NodeType GetNodeType() { return LIGHT; }

protected:
	float ambient = 1.0f;
	float diffuse = 1.0f;
	float specular = 1.0f;
	Vector3 colour = Vector3(1.0f, 1.0f, 1.0f);
};

