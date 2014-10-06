#pragma once

#include "TMesh.h"

class Triangle : public TMesh {
public:
	Triangle(const V3 &p0, const V3 &c0, const V3 &t0, const V3 &p1, const V3 &c1, const V3 &t1, const V3 &p2, const V3 &c2, const V3 &t2);
};

