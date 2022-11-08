#pragma once
#include "Light.h"
class DirectionalLight : public Light {
public:
	DirectionalLight() = default;
	DirectionalLight(Vector3 direction) : Light(), direction(direction) {}
	DirectionalLight(Vector3 colour, Vector3 direction) : Light(colour), direction(direction) {}
protected:
	Vector3 direction;
};

