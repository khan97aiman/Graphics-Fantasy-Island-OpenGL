#pragma once
#include "Matrix4.h"
#include "Plane.h"
class SceneNode;

enum Sides {
	RIGHT,
	LEFT,
	BOTTOM,
	TOP,
	NEAR,
	FAR,
};

class Frustum {
public:
	Frustum(void) = default;
	void BuildFustrum(const Matrix4& mvp); //view matrix is constantly changing on mouse input, hence we need to build frustum every frame
	bool IsInsideFrustum(SceneNode& n);
protected:
	Plane planes[6];
};

