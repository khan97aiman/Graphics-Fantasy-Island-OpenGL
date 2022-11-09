#include "Frustum.h"

void Frustum::BuildFustrum(const Matrix4& mvp) {
    Vector3 xaxis = Vector3(mvp.values[0], mvp.values[4], mvp.values[8]);
    Vector3 yaxis = Vector3(mvp.values[1], mvp.values[5], mvp.values[9]);
    Vector3 zaxis = Vector3(mvp.values[2], mvp.values[6], mvp.values[10]);
    Vector3 waxis = Vector3(mvp.values[3], mvp.values[7], mvp.values[11]);
    planes[RIGHT] = Plane(waxis - xaxis, (mvp.values[15] - mvp.values[12]), true);
    planes[LEFT] = Plane(waxis + xaxis, (mvp.values[15] + mvp.values[12]), true);
    planes[BOTTOM] = Plane(waxis + yaxis, (mvp.values[15] + mvp.values[13]), true);
    planes[TOP] = Plane(waxis - yaxis, (mvp.values[15] - mvp.values[13]), true);
    planes[zNEAR] = Plane(waxis + zaxis, (mvp.values[15] + mvp.values[14]), true);
    planes[zFAR] = Plane(waxis - zaxis, (mvp.values[15] - mvp.values[14]), true);
}

bool Frustum::IsInsideFrustum(GeometryNode& n) {
    for (int p = 0; p < 6; ++p) {
        if (!planes[p].IsSphereInPlane(n.GetWorldTransform().
            GetPositionVector(), n.GetBoundingRadius())) {
            return false; // scenenode is outside this plane!
        }
    }
    return true;
}
