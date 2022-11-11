#pragma once
#include "Light.h"
/*
* For more Information on Point Lights, read https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
* constant, linear and quadratic are light's attenuation proportions
*/
class PointLight : public Light {
public:
	PointLight() = default;
	PointLight(Vector3 position) : position(position) {}
	PointLight(Vector3 colour, Vector3 position) : Light(colour), position(position) {}
	PointLight(Vector3 position, float constant, float linear, float quadratic) {
		this->position = position;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}
	PointLight(Vector3 colour, Vector3 position, float constant, float linear, float quadratic) : Light(colour) {
		this->position = position;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}
	virtual NodeType GetNodeType() { return POINTLIGHT; }

public:
	Vector3 position;
	float constant = 1.0f;		//attenuation unaffected by distance
	float linear = 0.001f;		//attenuation is inversely proportional to distance
	float quadratic = 0.000001f;	//attenuation is inversely proportional to square of distance
};

