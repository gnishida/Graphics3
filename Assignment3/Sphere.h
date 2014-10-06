#pragma once

#include "TMesh.h"

class Sphere : public TMesh {
public:
	Sphere(float radius, const V3 &c, int nstack = 20, int nslice = 40);
};

