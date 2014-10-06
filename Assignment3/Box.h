#pragma once

#include "TMesh.h"

class Box : public TMesh {
public:
	Box(const V3 &p0, const V3 &p1, const V3 &col);
};

