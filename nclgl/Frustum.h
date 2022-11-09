#pragma once
#include "Matrix4.h"
#include "Plane.h"
#include "GeometryNode.h"


enum Sides {
	RIGHT,
	LEFT,
	BOTTOM,
	TOP,
	zNEAR,
	zFAR
};

class Frustum {
public:
	Frustum(void) = default;
	void BuildFustrum(const Matrix4& mvp); //view matrix is constantly changing on mouse input, hence we need to build frustum every frame
	bool IsInsideFrustum(GeometryNode& n);
protected:
	Plane planes[6];
};

