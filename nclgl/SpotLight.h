#pragma once
#include "PointLight.h"

class SpotLight : public PointLight {
public:
	SpotLight() = default;
	SpotLight(Vector3 position, Vector3 axis, float cutOff) : PointLight(position) {
		this->axis = axis;
		this->cutOff = cutOff;
	}
	SpotLight(Vector3 colour, Vector3 position, Vector3 axis, float cutOff) : PointLight(colour, position) {
		this->axis = axis;
		this->cutOff = cutOff;
	}
	SpotLight(Vector3 position) : PointLight(position) {}
	SpotLight(Vector3 colour, Vector3 position) : PointLight(colour, position) {}
	SpotLight(Vector3 position, float constant, float linear, float quadratic) : PointLight(position, constant, linear, quadratic) {}
	SpotLight(Vector3 colour, Vector3 position, float constant, float linear, float quadratic) : PointLight(colour, position, constant, linear, quadratic) {}
	virtual NodeType GetNodeType() { return SPOT; }
public:
	Vector3 axis = Vector3(0.0f, 1.0f, 0.0f);	// this is the axis of the spotlight, points in this direction receives maximum illumination
	float cutOff = 45.0f;						// cut off angle in degrees, fragment shader will convert this angle to radians
};

