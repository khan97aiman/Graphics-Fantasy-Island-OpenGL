#pragma once
#include "Vector3.h"

class Plane {
public:
	Plane() = default;
	Plane(const Vector3& normal, float distance, bool normalise = false);
	bool IsSphereInPlane(const Vector3& position, float radius) const;
protected:
	Vector3 normal;
	float distance;
};

