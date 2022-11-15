#pragma once
#include "Mesh.h"
class HeightMap : public Mesh {
public:
	HeightMap(const std::string & name, bool seaLevel = false);
	~HeightMap(void) {};
	Vector3 GetHeightmapSize() const { return heightmapSize; }
protected:
	Vector3 heightmapSize;
};

